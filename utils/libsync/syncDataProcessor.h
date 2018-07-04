#pragma once

#ifndef LIBSYNC_PROCESSOR_H
#define LIBSYNC_PROCESSOR_H
#include "syncData.h"
#include "we_client.h"


namespace SYNC
{
    using namespace BRM;
    typedef std::queue<SyncData> SyncDataQueue;

    // writeengineServer 内
    class SyncDataProcessor :public SyncBase
    {
        SyncDataQueue dataQueue;
        SyncDataProcessor();
    public:
        static SyncDataProcessor * makeSyncDataProcessor();
        void onReceiveKeepAlive(const messageqcpp::IOSocket& ios, ByteStream& Ibs);
        void msgProc(ByteStream::byte msgId, messageqcpp::IOSocket& ios, messageqcpp::ByteStream &);
        virtual void msgProc(WEClient *, messageqcpp::SBS &);
        void addSyncData(SyncData &syncData);
        SyncData * getNextSyncData();
        void run();
        uint16_t connectedWEServers() const { return syncThreads.size(); }
    };
    class SyncDataThread:public WEClient //一个线程连接一个wes
    {
        SyncDataProcessor * syncProc;
        SyncData syncData;
        SYNC_DATA_STATE dataState;
    public:
        SyncDataThread(SyncDataProcessor * syncProc,int PrgmID,uint16_t pmId);
        //void operator()();
        ~SyncDataThread() ;
    };
    
    extern SyncDataProcessor * syncProc;

};
#endif
