#pragma once
#include "syncData.h"
#ifndef LIBSYNC_PROCESSOR_H
#define LIBSYNC_PROCESSOR_H
namespace sync
{
    using namespace BRM;
    typedef std::queue<SyncData> SyncDataQueue;

    static SyncDataProcessor * syncProc;
    boost::mutex syncProcMutex;
    class SyncDataProcessor :public SyncBase
    {
        SyncDataQueue dataQueue;
        SyncDataProcessor();
    public:
        static SyncDataProcessor * makeSyncDataProcessor()
        {
            boost::mutex::scoped_lock mgrLock(syncProcMutex);
            if (syncProc)
            {
                mgrLock.unlock();
                return syncProc;
            }
            syncProc = new SyncDataProcessor();
            syncProc->reader = new boost::thread(*syncProc);
            mgrLock.unlock();
            return syncProc;
        };
        void onReceiveKeepAlive(const messageqcpp::IOSocket& ios, ByteStream& Ibs);
        void msgProc(ByteStream::byte msgId, const messageqcpp::IOSocket& ios, messageqcpp::ByteStream &);
        virtual void msgProc(WEClient *, messageqcpp::SBS &);
        void addSyncData(SyncData &syncData);
        SyncData * getNextSyncData();
        void operator()();
        uint16_t connectedWEServers() const { return syncThreads.size(); }
    };
    class SyncDataThread:public WEClient //一个线程连接一个wes
    {
        SyncDataProcessor * syncProc;
        SyncData syncData;
        SYNC_DATA_STATE dataState;
    public:
        SyncDataThread(SyncDataProcessor * syncProc, messageqcpp::IOSocket *sock);
        //void operator()();
        ~SyncDataThread() { WEClient ::~WEClient(); };
    };

};
#endif
