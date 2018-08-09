#pragma once

#ifndef LIBSYNC_PROCESSOR_H
#define LIBSYNC_PROCESSOR_H
#include "syncData.h"
#include "SyncClient.h"


namespace SYNC
{
    using namespace BRM;
    typedef std::queue<SyncData> SyncDataQueue;

    // writeengineServer ��
    class SyncDataProcessor :public SyncBase
    {
        SyncDataQueue dataQueue;
        SyncDataProcessor();
    public:
        static SyncDataProcessor * makeSyncDataProcessor();
        void onReceiveKeepAlive(const messageqcpp::IOSocket& ios, ByteStream& Ibs);
        void msgProc(ByteStream::byte msgId, messageqcpp::IOSocket& ios, messageqcpp::ByteStream &);
        virtual void msgProc(SyncClient *, messageqcpp::SBS &);
        void addSyncData(SyncData &syncData);
        SyncData * getNextSyncData();
        void run();
        uint16_t connectedWEServers() const { return syncThreads.size(); }
    };
    class SyncDataThread:public SyncClient //һ���߳�����һ��wes
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
