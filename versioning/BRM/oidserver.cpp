/* Copyright (C) 2014 EryDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

   /*****************************************************************************
    * $Id: objectidmanager.cpp 7409 2011-02-08 14:38:50Z rdempsey $
    *
    ****************************************************************************/

    /** @file
     * This file implements OIDServer
     *
     * The OIDServer is responsible for allocating and deallocating
     * Object IDs from a 24-bit space consistently across all processes & threads.
     * The expected allocation size is 1 or 2 OIDs, and the usage pattern is
     * unknown.  The OID space is described by a 16Mbit bitmap file on disk
     * and a brief free list.  Accesses are synchronized by using flock().
     *
     * This class must implement a high degree of correctness.  However, it
     * also requires file IO.  Most functions throw an exception if a hard IO error
     * occurs more than MaxRetries times in a row.  Right now the code makes
     * no attempt to back out changes that occured before the error although it
     * may be possible to do so for certain errors.  Probably the best course of
     * action would be to halt the system if an exception is thrown here
     * to prevent database corruption resulting allocation of OIDs from a
     * possibly corrupt OID bitmap.  The OID bitmap can be rebuilt at system
     * startup if necessary (need to write a tool to do that still).
     *
     * There are a few checks to verify the safety of allocations and
     * deallocations & the correctness of this implementation in general.
     * Those errors will throw logic_error.  IO errors will throw
     * ios_base::failure.
     *
     * There are probably oodles of optimizations possible given this implmementation.
     * For example:
     * 		- make fullScan() construct a freelist
     *		- sorting & coalescing free list entries will raise the hit rate
     *		  (at what cost?)
     *		- implement a bias for high numbered OIDs in the free list to reduce
     *	 	  the number of fullScans searching far in the bitmap
     *		- avoid the extra read in flipOERYDBlock when called by fullscan
     * 		- singleton object operating entirely in memory (no file IO)
     *			- singleton object using only a free list (no bitmap)
     *		- a different implementation altogether.
     *
     * Time permitting we might look into those things.
     */

#define BRMOIDSVR_DLLEXPORT
#include "oidserver.h"
#include "liboamcpp.h"
#undef BRMOIDSVR_DLLEXPORT

#include "configcpp.h"
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <stdexcept>
#if __linux__
#include <values.h>
#include <sys/file.h>
#endif
#include <sstream>
     //#define NDEBUG
#include <cassert>
#include <boost/thread/thread.hpp>

#include "ERYDBDataFile.h"
#include "ERYDBPolicy.h"

using namespace std;
using namespace erydbdatafile;

#include "dbrm.h"

#ifndef O_BINARY
#  define O_BINARY 0
#endif
#ifndef O_DIRECT
#  define O_DIRECT 0
#endif
#ifndef O_LARGEFILE
#  define O_LARGEFILE 0
#endif
#ifndef O_NOATIME
#  define O_NOATIME 0
#endif

namespace
{
    boost::mutex CtorMutex;

    class EOFException : public exception {
    };
}

namespace BRM {

    boost::mutex OIDServer::fMutex;

#if 0
    void OIDServer::lockFile() const {
        int err, errCount;

        int errnoSave = 0;
        for (errCount = 0, err = -1; err != 0 && errCount < MaxRetries;) {
            err = flock(fFd, LOCK_EX);
            if (err < 0 && errno != EINTR) {  // EINTR isn't really an error
                errCount++;
                errnoSave = errno; // save errno because perror may overwrite
                perror("OIDServer::lockFile(): flock (retrying)");
            }
        }
        if (errCount == MaxRetries) {
            ostringstream oss;
            oss << "OIDServer::lockFile(): flock error:  " << strerror(errnoSave);
            throw ios_base::failure(oss.str());
        }
    }
#endif

    void OIDServer::writeData(uint8_t *buf, off_t offset, int size) const {
        int errCount, err, progress;
        off_t seekerr = -1;

        if (size == 0)
            return;

        if (ERYDBPolicy::useHdfs()) {
            for (errCount = 0; errCount < MaxRetries && seekerr != offset; errCount++) {
                seekerr = fFp->seek(offset, SEEK_SET);
                if (seekerr >= 0)
                    seekerr = fFp->tell(); // ERYDBDataFile may use fseek for seek.
                if (seekerr < 0)
                    perror("OIDServer::writeDataHdfs(): lseek");
            }
            if (errCount == MaxRetries)
                throw ios_base::failure("OIDServer::writeDataHdfs(): lseek failed "
                    "too many times");

            for (progress = 0, errCount = 0; progress < size && errCount < MaxRetries;) {
                err = fFp->write(&buf[progress], size - progress);
                if (err < 0) {
                    if (errno != EINTR) {  // EINTR isn't really an error
                        errCount++;
                        perror("OIDServer::writeDataHdfs(): write (retrying)");
                    }
                } else
                    progress += err;
            }

            fFp->tell();
        } else {
            for (errCount = 0; errCount < MaxRetries && seekerr != offset; errCount++) {
                seekerr = lseek(fFd, offset, SEEK_SET);
                if (seekerr < 0)
                    perror("OIDServer::writeData(): lseek");
            }
            if (errCount == MaxRetries)
                throw ios_base::failure("OIDServer::writeData(): lseek failed "
                    "too many times");

            for (progress = 0, errCount = 0; progress < size && errCount < MaxRetries;) {
                err = write(fFd, &buf[progress], size - progress);
                if (err < 0) {
                    if (errno != EINTR) {  // EINTR isn't really an error
                        errCount++;
                        perror("OIDServer::writeData(): write (retrying)");
                    }
                } else
                    progress += err;
            }
        }

        if (errCount == MaxRetries)
            throw ios_base::failure("OIDServer::writeData(): write error");
    }

    void OIDServer::readData(uint8_t *buf, off_t offset, int size) const {
        int errCount, err, progress;
        off_t seekerr = -1;

        if (size == 0)
            return;

        if (ERYDBPolicy::useHdfs()) {
            for (errCount = 0; errCount < MaxRetries && seekerr != offset; errCount++) {
                seekerr = fFp->seek(offset, SEEK_SET);
                if (seekerr >= 0)
                    seekerr = fFp->tell(); // ERYDBDataFile may use fseek for seek.
                if (seekerr < 0)
                    perror("OIDServer::readDataHdfs(): lseek");
            }
            if (errCount == MaxRetries)
                throw ios_base::failure("OIDServer::readDataHdfs(): lseek failed "
                    "too many times");

            for (progress = 0, errCount = 0; progress < size && errCount < MaxRetries;) {
                err = fFp->read(&buf[progress], size - progress);
                if (err < 0) {
                    if (errno != EINTR) {  // EINTR isn't really an error
                        errCount++;
                        perror("OIDServer::readDataHdfs(): read (retrying)");
                    }
                } else if (err == 0)
                    throw EOFException();
                else
                    progress += err;
            }
        } else {
            for (errCount = 0; errCount < MaxRetries && seekerr != offset; errCount++) {
                seekerr = lseek(fFd, offset, SEEK_SET);
                if (seekerr < 0)
                    perror("OIDServer::readData(): lseek");
            }
            if (errCount == MaxRetries)
                throw ios_base::failure("OIDServer::readData(): lseek failed "
                    "too many times");

            for (progress = 0, errCount = 0; progress < size && errCount < MaxRetries;) {
                err = read(fFd, &buf[progress], size - progress);
                if (err < 0) {
                    if (errno != EINTR) {  // EINTR isn't really an error
                        errCount++;
                        perror("OIDServer::readData(): read (retrying)");
                    }
                } else if (err == 0)
                    throw EOFException();
                else
                    progress += err;
            }
        }

        if (errCount == MaxRetries)
            throw ios_base::failure("OIDServer::readData(): read error");
    }

    void OIDServer::initializeBitmap() const {
        int i;
        struct FEntry *h1;
        string stmp;
        int64_t ltmp= USER_OBJECT_ID;
        int firstOID;
        config::Config *conf;
        conf = config::Config::makeConfig();
        try {
            stmp = conf->getConfig("OIDManager", "FirstOID");
        } catch (exception&) {
        }
        if (!stmp.empty())
            ltmp = config::Config::fromText(stmp);
       
        if (ltmp > numeric_limits<int32_t>::max() || ltmp < USER_OBJECT_ID) {
            ltmp = USER_OBJECT_ID;
        }
        firstOID = static_cast<int>(ltmp);
        boost::mutex::scoped_lock lk(fMutex);
        writeData((uint8_t*)&firstOID, 0, HeaderSize);
        //short sz = 0;
        ///* append a 16-bit 0 to indicate 0 entries in the vboid->dbroot mapping */
        //writeData((uint8_t*)&sz, HeaderSize, 2);
    }

    OIDServer::OIDServer() : fFp(NULL), fFd(-1) , currentOID(0){
        boost::mutex::scoped_lock lk(CtorMutex);

        config::Config* conf;
        string tmp;
        ostringstream os;

        conf = config::Config::makeConfig();
        fFilename = conf->getConfig("OIDManager", "OERYDBitmapFile");

        if (fFilename.empty()) {
            os << "OIDServer: <OIDManager><OERYDBitmapFile> must exist in the config file";
            log(os.str());
            throw runtime_error(os.str());
        }

        if (ERYDBPolicy::useHdfs()) {
            if (!ERYDBPolicy::exists(fFilename.c_str())) { //no bitmap file
                BRM::DBRM em;
                if (!em.isEMEmpty()) {
                    os << "Extent Map not empty and " << fFilename << " not found. Setting system to read-only";
                    cerr << os.str() << endl;
                    log(os.str());
                    em.setReadOnly(true);
                    throw runtime_error(os.str());
                }

                fFp = ERYDBDataFile::open(ERYDBPolicy::getType(fFilename.c_str(), ERYDBPolicy::WRITEENG),
                    fFilename.c_str(), "w+b", 0, 1);

                if (!fFp) {
                    os << "Couldn't create oid bitmap file " << fFilename << ": " <<
                        strerror(errno);
                    log(os.str());
                    throw ios_base::failure(os.str());
                }
#ifndef _MSC_VER
                //FIXME:
                //fchmod(fFd, 0666);   // XXXPAT: override umask at least for testing
                if (fFp)
                    chmod(fFilename.c_str(), 0664);   // XXXPAT: override umask at least for testing
#endif
                try {
                    initializeBitmap();
                } catch (...) {
                    delete fFp;
                    fFp = NULL;
                    throw;
                }
            } else {
                fFp = ERYDBDataFile::open(ERYDBPolicy::getType(fFilename.c_str(), ERYDBPolicy::WRITEENG),
                    fFilename.c_str(), "r+b", 0, 1);
                if (!fFp) {
                    ostringstream os;
                    os << "Couldn't open oid bitmap file" << fFilename << ": " <<
                        strerror(errno);
                    log(os.str());
                    throw ios_base::failure(os.str());
                }
            }
        } else {
            if (access(fFilename.c_str(), F_OK) != 0) //no bitmap file
            {
                BRM::DBRM em;
                if (!em.isEMEmpty()) {
                    os << "Extent Map not empty and " << fFilename << " not found. Setting system to read-only";
                    cerr << os.str() << endl;
                    log(os.str());
                    em.setReadOnly(true);
                    throw runtime_error(os.str());
                }
            }

            fFd = open(fFilename.c_str(), O_CREAT | O_EXCL | O_RDWR | O_BINARY, 0664);
            if (fFd >= 0) {
#ifndef _MSC_VER
                //FIXME:
                fchmod(fFd, 0666);   // XXXPAT: override umask at least for testing
#endif
                try {
                    initializeBitmap();
                } catch (...) {
                    close(fFd);
                    throw;
                }
            } else if (errno == EEXIST) {
                fFd = open(fFilename.c_str(), O_RDWR | O_BINARY);
                if (fFd < 0) {
                    os << "Couldn't open oid bitmap file " << fFilename << ": " <<
                        strerror(errno);
                    log(os.str());
                    throw ios_base::failure(os.str());
                }
            } else {
                os << "Couldn't create oid bitmap file " << fFilename << ": " <<
                    strerror(errno);
                log(os.str());
                throw ios_base::failure(os.str());
            }
        }
//        loadVBOIDs();
    }

    OIDServer::~OIDServer() {
        if (fFd >= 0)
            close(fFd);

        delete fFp;
        fFp = NULL;
    }

    //void OIDServer::loadVBOIDs() {
    //    uint16_t size;
    //    try {
    //        readData((uint8_t *)&currentOID, 0, 2);
    //        readData((uint8_t *)&size, HeaderSize, 2);
    //    } catch (EOFException &e) {
    //        /* convert old OID bitmap */
    //        size = 0;
    //        writeData((uint8_t *)&size, HeaderSize, 2);
    //    }

    //    if (size == 0)
    //        return;
    //    vbOidDBRootMap.resize(size);
    //    readData((uint8_t *)&vbOidDBRootMap[0], HeaderSize + 2, size * 2);

    //    //cout << "loaded " << size << " vboids: ";
    //    //for (uint32_t i = 0; i < size; i++)
    //    //	cout << (int) vbOidDBRootMap[i] << " ";
    //    //cout << endl;
    //}


    //int OIDServer::allocVBOID(uint16_t dbroot) {
    //    int ret;
    //    uint32_t offset;
    //    ret = vbOidDBRootMap.size();
    //    offset = HeaderSize + 2 + (vbOidDBRootMap.size() * sizeof(short));
    //    vbOidDBRootMap.push_back(dbroot);
    //    try {
    //        uint16_t size = vbOidDBRootMap.size();
    //        boost::mutex::scoped_lock lk(fMutex);
    //        writeData((uint8_t *)&size, HeaderSize, sizeof(short));
    //        writeData((uint8_t *)&dbroot, offset, sizeof(short));
    //    } catch (...) {
    //        vbOidDBRootMap.pop_back();
    //        throw;
    //    }
    //    if (ERYDBPolicy::useHdfs())
    //        fFp->flush();
    //    return ret;
    //}

    //int OIDServer::getDBRootOfVBOID(uint32_t vbOID) {
    //    //cout << "getDBRootOfVBOID, vbOID = " << vbOID << " size = " << vbOidDBRootMap.size() << endl;
    //    if (vbOID >= vbOidDBRootMap.size())
    //        return -1;
    //    return vbOidDBRootMap[vbOID];
    //}

    //const vector<uint16_t> & OIDServer::getVBOIDToDBRootMap() {
    //    return vbOidDBRootMap;
    //}

    //int OIDServer::getVBOIDOfDBRoot(uint32_t dbRoot) {
    //    uint32_t i;

    //    for (i = 0; i < vbOidDBRootMap.size(); i++)
    //        if (vbOidDBRootMap[i] == dbRoot)
    //            return i;
    //    return -1;
    //}

    int OIDServer::allocOIDs(int num) {
        int bestMatchBegin = this->currentOID;
        this->currentOID += num; 
        writeData(reinterpret_cast<uint8_t*>(&currentOID), 0, HeaderSize);
        if (ERYDBPolicy::useHdfs())
            fFp->flush();
        return bestMatchBegin;
    }

    void OIDServer::returnOIDs(int start, int end) const {
    }

    int OIDServer::size() const {
        return currentOID;
    }

    const string OIDServer::getFilename() const {
        return fFilename;
    }

}  // namespace
