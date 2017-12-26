#pragma once
#ifndef LIBSYNC_DATA_H
#define LIBSYNC_DATA_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <map>
using namespace std;

#include "logicalpartition.h"
#include "bytestream.h"
#include "rwlock_local.h"
#include "sessionmanager.h"
#include "socketclosed.h"
#include "alarmglobal.h"
#include "alarmmanager.h"
#include "liboamcpp.h"
#include "stopwatch.h"
#include <queue>
#include "masterdbrmnode.h"
#include "messagequeuepool.h"
#include "syncDataProcessor.h"
#if defined(_MSC_VER) && defined(xxxBRMTYPES_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
#include "license.h"
using namespace Common;
#include "we_client.h"

namespace sync {
    using namespace BRM;
    struct SyncData
    {
        uint16_t srcDbr;
        uint16_t destDbr;
        int extentIndex;
        std::uint64_t lbid;
        EXPORT void serialize(messageqcpp::ByteStream &bs) const;
        EXPORT void deserialize(messageqcpp::ByteStream &bs);
    };
    EXPORT std::ostream & operator<<(std::ostream &, const SyncData &);
    EXPORT std::istream & operator>>(std::istream &, SyncData &);
    inline messageqcpp::ByteStream& operator<<(messageqcpp::ByteStream& bs, const SyncData& s)
    {
        s.serialize(bs); return bs;
    };
    inline messageqcpp::ByteStream& operator>>(messageqcpp::ByteStream& bs, SyncData& s)
    {
        s.deserialize(bs); return bs;
    };


    enum SYNC_STATE
    {
        SYNC_WAIT=1,
        SYNC_START,
        SYNC_SYNCING,
        SYNC_SUCCESS,
        SYNC_CANCEL,
        SYNC_STOP
    };
    struct SYNC_DATA_STATE
    {
        SYNC_DATA_STATE(SyncData &data) :extData(data),  state(SYNC_WAIT) {};
        SyncData &extData;
        DateTime dt;
        SYNC_STATE state;
    };
    struct WEClientThread
    {
        WEClient* we;
        boost::thread* th;
    };
    typedef std::vector<SyncData> SyncDataList;
    typedef std::vector<SYNC_DATA_STATE> SYNC_DATA_STATEList;
    typedef boost::shared_ptr<std::map<std::uint64_t, SYNC_DATA_STATE> > SyncStatMap;
    typedef std::queue<SyncDataList> SyncListQueue;
    typedef std::map<uint16_t, WEClientThread > ClientThread;
    typedef std::map<uint16_t, bool > ClientFlag;
     
    static SyncManager * syncMgr;
    boost::mutex syncMgrMutex;
    class SyncBase
    {
    public:
        bool running;
        int fPMCount;
        OamCache::UintUintMap fDbRootPMMap;
        BRM::DBRM* dbrm;
        ClientThread syncThreads;
        boost::mutex fDataQMutex;
        ClientFlag pmFlag;
        boost::thread *reader;
        int syncThreadNums;


        SyncBase();
        virtual void msgProc(WEClient *, messageqcpp::SBS &);
        void closeWEclient(uint16_t pmId);
        void close();
    };
    class SyncManager:public SyncBase
    {
    private:
        SyncManager(): curIndex(-1), SyncBase(){
            oam::OamCache * oamCache = oam::OamCache::makeOamCache();
        };
        SyncListQueue fDataQueue;
        SyncStatMap syncStates; 
        SyncDataList curSyncDataList;
        int curIndex;

    public:
        static SyncManager * makeSyncManager() {
            boost::mutex::scoped_lock mgrLock(syncMgrMutex);
            if (syncMgr)
            {
                mgrLock.unlock();
                return syncMgr;
            }
            syncMgr = new SyncManager();
            syncMgr->reader = new boost::thread(*syncMgr);
            mgrLock.unlock();
            return syncMgr;
        };
        void addSyncData(SyncDataList &syncData);
        SYNC_DATA_STATE * setSyncDataState(SyncData &syncData, SYNC_STATE state);
        SYNC_DATA_STATE getSyncDataState(SyncData & syncData);
        SYNC_DATA_STATEList getAllSyncDataState(int state=0);
        SyncData * getNextSyncData();
        virtual void msgProc(WEClient *, messageqcpp::SBS &);

        void operator()();
        uint16_t connectedWEServers() const { return syncThreads.size(); }
    };
   
   
};
#endif
