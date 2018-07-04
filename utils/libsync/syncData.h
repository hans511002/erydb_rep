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

#if defined(_MSC_VER) && defined(xxxBRMTYPES_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
#include "license.h"
using namespace Common;


namespace SYNC {
    using namespace BRM;
    //同步数据摘要对象
    struct SyncData
    {
        //em
        DBROOTS_struct dbRoot;
        int extentIndex;

        //blocks



        
        unsigned long long lbid;
        uint32_t fbo;
        uint32_t oid;
        int extentIndex;
        int size;
        bool locked;
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
        SYNC_DATA_STATE(){};
        SYNC_DATA_STATE(SyncData &data) :extData(data),  state(SYNC_WAIT) {};
        SyncData extData;
        DateTime dt;
        SYNC_STATE state;
    };
    class WEClient;
    struct WEClientThread
    {
        WEClient* we;
        boost::thread* th;
    };
    typedef std::vector<SyncData> SyncDataList;
    typedef std::vector<SYNC_DATA_STATE> SYNC_DATA_STATEList;
    typedef boost::shared_ptr<std::map<uint64_t, SYNC_DATA_STATE> > SyncStatMap;
    typedef std::queue<SyncDataList> SyncListQueue;
    typedef std::map<uint16_t, WEClientThread > ClientThread;
    typedef std::map<uint16_t, bool > ClientFlag;
     

    extern boost::mutex syncMgrMutex;
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

    //master内同步管理
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
        static SyncManager * makeSyncManager();
        void addSyncData(SyncDataList &syncData);
        SYNC_DATA_STATE * setSyncDataState(SyncData &syncData, SYNC_STATE state);
        SYNC_DATA_STATE getSyncDataState(SyncData & syncData);
        SYNC_DATA_STATEList getAllSyncDataState(int state=0);
        SyncData * getNextSyncData();
        virtual void msgProc(WEClient *, messageqcpp::SBS &);

        void run();
        uint16_t connectedWEServers() const { return syncThreads.size(); }
    };
    extern SyncManager * syncMgr; 
   
};
#endif
