#include <boost/thread/condition.hpp>
#include <boost/scoped_array.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
using namespace boost;
#include "bytestream.h"
#include "messagequeue.h"
using namespace messageqcpp;

#include "we_messages.h"
#include "we_brmrprtparser.h"

#include "installdir.h"
#include "syncData.h"
#include "syncDataProcessor.h"

namespace SYNC
{
    SyncManager * syncMgr=0;
    boost::mutex syncMgrMutex;
        
    struct SyncManagerThread{
        SyncManager * mgr;
        SyncManagerThread(SyncManager *_mgr):mgr(_mgr){};
        void operator()(){
            mgr->run();
        };
    };
    SyncManager * SyncManager::makeSyncManager() {
        mutex::scoped_lock mgrLock(syncMgrMutex);
        if (syncMgr)
        {
            mgrLock.unlock();
            return syncMgr;
        }
        syncMgr = new SyncManager();
        syncMgr->reader = new boost::thread(SyncManagerThread(syncMgr));
        mgrLock.unlock();
        return syncMgr;
    };
    SyncBase::SyncBase() :running(true), syncThreadNums(1), reader(0)
    {
        oam::OamCache * oamCache = oam::OamCache::makeOamCache();
        fDbRootPMMap = oamCache->getDBRootToPMMap();
        fPMCount = oamCache->getPMCount();
        dbrm = new BRM::DBRM();
    };
    void SyncBase::closeWEclient(uint16_t pmId)
    {
        mutex::scoped_lock mgrLock(syncMgrMutex);
        ClientThread::iterator it = syncThreads.find(pmId);
        if (it != syncThreads.end())
        {
            it->second.we->close();
            it->second.th->join();
            delete it->second.th;
            delete it->second.we;
            syncThreads.erase(pmId);
        }
        mgrLock.unlock();
    }
    void SyncBase::close()
    {
        mutex::scoped_lock mgrLock(syncMgrMutex);
        running = false;
        syncMgr = 0;
        ClientThread::iterator it;
        for (it = syncThreads.begin(); it != syncThreads.end(); it++)
        {
            it->second.we->close();
            it->second.th->join();
            delete it->second.th;
            delete it->second.we;
        }
        syncThreads.clear();
        delete reader;
        mgrLock.unlock();
    };
    void SyncBase::msgProc(SyncClient * wc, messageqcpp::SBS &bs)
    {

    };


    void SyncData::serialize(messageqcpp::ByteStream &bs) const
    {
        bs << srcDbr;
        bs << destDbr;
        bs << extentIndex;
        bs << lbid;
    };

    void SyncData::deserialize(messageqcpp::ByteStream &bs)
    {
        bs >> srcDbr;
        bs >> destDbr;
        bs >> extentIndex;
        bs >> lbid;
    };

    std::ostream & operator<<(std::ostream &os, const SyncData & a)
    {
        os << a.srcDbr;
        os << "," << a.destDbr;
        os << "," << a.extentIndex;
        os << "," << a.lbid;
        return os;
    };
    std::istream & operator>>(std::istream & is, SyncData &a)
    {
        is>> a.srcDbr;
        is.ignore();
        is  >> a.destDbr;
        is.ignore();
        is >> a.extentIndex;
        is.ignore();
        is >> a.lbid;
        return is;
    };


}