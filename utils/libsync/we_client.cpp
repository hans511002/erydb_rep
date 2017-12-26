/* Copyright (C) 2014 EryDB, Inc.
 * Copyright (C) 2016 MariaDB Corporation.

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

// $Id: WEClient.h 525 2010-01-19 23:18:05Z xlou $
//
/** @file */

#include <sstream>
#include <stdexcept>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <unistd.h>
#ifndef _MSC_VER
#include <arpa/inet.h>
#if __FreeBSD__
#include <sys/socket.h>
#endif
#endif
using namespace std;

#include <boost/thread/mutex.hpp>
using namespace boost;

#include "messagequeue.h"


#include "configcpp.h"
using namespace config;

#include "errorids.h"
#include "exceptclasses.h"
#include "messagelog.h"
#include "messageobj.h"
#include "loggingid.h"
using namespace logging;

#include "liboamcpp.h"
#include "alarmmanager.h"
using namespace alarmmanager;
using namespace oam;

#include "oamcache.h"
#include "we_client.h"
#include "we_messages.h"
using namespace WriteEngine;

#include "atomicops.h"


namespace sync
{
 void  writeToLog(int l,const char* file, int line, const string& msg, LOG_TYPE logto = LOG_TYPE_INFO)
  {
        LoggingID lid(l);
        MessageLog ml(lid);
        Message::Args args;
        Message m(0);
        args.add(file); 
        args.add("@");
        args.add(line);
        args.add(msg);
        m.format(args); 
	switch (logto)
	{
        	case LOG_TYPE_DEBUG:	ml.logDebugMessage(m); break;		
        	case LOG_TYPE_INFO: 	ml.logInfoMessage(m); break;	
        	case LOG_TYPE_WARNING:	ml.logWarningMessage(m); break;	
        	case LOG_TYPE_ERROR:	ml.logErrorMessage(m); break;	
        	case LOG_TYPE_CRITICAL:	ml.logCriticalMessage(m); break;	
	}
  }
    
  WEClient::WEClient(SyncBase * _syncMgr, int PrgmID, uint16_t pmId) :syncMgr(_syncMgr),fPrgmID(PrgmID),fpmId(pmId)
  {
	closingConnection = 0;
    Setup();
  }

  WEClient::~WEClient()
  {
      Close();
  }

void WEClient::Setup()
{
    fBusy=(true);
    joblist::ResourceManager rm;
    oam::Oam oam;
    string ipAddress;
    ModuleTypeConfig moduletypeconfig; 
	try {
    	oam.getSystemConfig("pm", moduletypeconfig);
	} catch (...) {
		writeToLog(fPrgmID,__FILE__, __LINE__, "oam.getSystemConfig error, unknown exception", LOG_TYPE_ERROR);
		throw runtime_error("Setup failed");
	}  
    char buff[32];
	ByteStream bs, bsRead;
	bs << (ByteStream::byte) WE_SYNC_SRV_KEEPALIVE;
	bs << (ByteStream::octbyte) fpmId;
    //WE_SYNC_SRV_KEEPALIVE,
    //    WE_SYNC_REQ_DATA,
    //    WE_SYNC_RECIVE_DATA,
    //    WE_SYNC_ROLLBACK,
    //    WE_SYNC_SUCESS,
    //    WE_SYNC_RES_STATE 
    snprintf(buff, sizeof(buff), "pm%u_WriteEngineServer", fpmId);
    string fServer(buff);
    boost::shared_ptr<MessageQueueClient> cl(new MessageQueueClient(fServer, rm.getConfig()));
    boost::shared_ptr<boost::mutex> nl(new boost::mutex());
    //Bug 5224. Take out the retrys. If connection fails, we assume the server is down.
    try
    {
        if (cl->connect())
        {
            try
            {
                cl->write(bs);
            } catch (std::exception& ex1)
            {
                ostringstream oss;
                oss << "Write to WES during connect failed due to " << ex1.what();
                throw runtime_error(oss.str());
            }
            try
            {
                bsRead = cl->read();
                if (bsRead.length() == 0)
                    throw runtime_error("Got byte 0 during reading ");
            }
            catch (std::exception& ex2)
            {
                ostringstream oss;
                oss << "Read from WES during connect failed due to " << ex2.what() << " and this = " << this;
                throw runtime_error(oss.str());
            }
            client = cl;
            //cout << "connection is open. this = " << this << endl;
            //cout << "set up connection to mudule " << moduleID << endl;
            // assign the module name
            //ipAddress = sin_addr2String(cl->serv_addr().sin_addr);
            ipAddress = cl->addr2String();
            snprintf(buff, sizeof(buff), "pm%u", fpmId);
            string moduleName(buff);
            cl->moduleName(moduleName);
            //ostringstream oss;
            //oss << "WECLIENT: connected to " << fServer + " and this = " << this << " and pmcount is now " << pmCount;
            //writeToLog(fPrgmID,__FILE__, __LINE__, oss.str() , LOG_TYPE_DEBUG);
        } else
        {
            throw runtime_error("Connection refused");
        }
    } catch (std::exception& ex)
    {
        writeToLog(fPrgmID, __FILE__, __LINE__, "Could not connect to " + fServer + ": " + ex.what(), LOG_TYPE_ERROR);
        cerr << "Could not connect to " << fServer << ": " << ex.what() << endl;
    } catch (...)
    {
        writeToLog(fPrgmID, __FILE__, __LINE__, "Could not connect to " + fServer, LOG_TYPE_ERROR);
    }
}

int WEClient::Close()
{
    fBusy=(false);
	closingConnection = 1;
	ByteStream bs;
	bs << (ByteStream::byte) WE_SYNC_CLOSE_CONNECTION;
	write(bs);
    thReader->join();
 	//ostringstream oss;
	//oss << "WECLIENT: closed connection to wes and this = " << this << " and pmcount is now " << pmCount;
	//writeToLog(fPrgmID,__FILE__, __LINE__, oss.str() , LOG_TYPE_DEBUG);
    return 0;
}
void WEClient::operator()() {
	SBS sbs;
	try {
		while (fBusy)
		{
			//TODO: This call blocks so setting Busy() in another thread doesn't work here...
			sbs = client->read();
			if ( sbs->length() != 0 )
			{
				//cout << "adding data to connIndex " << endl;
                syncMgr->msgProc(*this,sbs);
			}
			else // got zero bytes on read, nothing more will come
			{
				if (closingConnection > 0)
				{
					return;
				}
				cerr << "WEC got 0 byte message for object " << this << endl;
				goto Error;
			}
		}
		return;
	} catch (std::exception& e)
	{
		cerr << "WEC Caught EXCEPTION: " << e.what() << endl;
		goto Error;
	}
	catch (...)
	{
		cerr << "WEC Caught UNKNOWN EXCEPT" << endl;
		goto Error;
	}
Error:
	// error condition! push 0 length bs to messagequeuemap and
	// eventually let jobstep error out.
	mutex::scoped_lock lk(fMlock); 
	sbs.reset(new ByteStream(0)); 
    mqe->queue.clear();
	(void)atomicops::atomicInc(&mqe->unackedWork[0]);
    mqe->queue.push(sbs); 
	lk.unlock(); 
	// reset the pmconnection map
	{
		mutex::scoped_lock onErrLock(fOnErrMutex);
		string moduleName = client->moduleName();
        client.reset();
		// send alarm
		ALARMManager alarmMgr;
//		string alarmItem = sin_addr2String(client->serv_addr().sin_addr);
		string alarmItem = client->addr2String();
		alarmItem.append(" WriteEngineServer");
		alarmMgr.sendAlarmReport(alarmItem.c_str(), oam::CONN_FAILURE, SET);
	}
	return;
} 
void WEClient::write(const messageqcpp::ByteStream &msg)
{ 
	client->write(msg);
}
  
}

// vim:ts=4 sw=4:
