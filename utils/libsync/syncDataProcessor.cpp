#include "we_messages.h"
#include "syncDataProcessor.h"

namespace SYNC
{
    SyncDataProcessor * syncProc=0;
    struct SyncDataProcessorThread{
        SyncDataProcessor * mgr;
        SyncDataProcessorThread(SyncDataProcessor *_mgr):mgr(_mgr){};
        void operator()(){
            mgr->run();
        };
    };
    SyncDataProcessor * SyncDataProcessor::makeSyncDataProcessor()
    {
        boost::mutex::scoped_lock mgrLock(syncMgrMutex);
        if (syncProc)
        {
            mgrLock.unlock();
            return syncProc;
        }
        syncProc = new SyncDataProcessor();
        syncProc->reader = new boost::thread(SyncDataProcessorThread(syncProc));
        mgrLock.unlock();
        return syncProc;
    };
    void SyncDataProcessor::run(){
        
        };


    SyncDataProcessor::SyncDataProcessor() {

    };
    void SyncDataProcessor::onReceiveKeepAlive(const messageqcpp::IOSocket& ios,ByteStream& Ibs){

    };
    void SyncDataProcessor::msgProc(ByteStream::byte msgId,messageqcpp::IOSocket& ios, messageqcpp::ByteStream &ibs) {
        switch (msgId)
        {
            case WriteEngine::WE_SYNC_SRV_KEEPALIVE:
            {
                onReceiveKeepAlive(ios, ibs);
                break;
            } 
            case WriteEngine::WE_SYNC_CLOSE_CONNECTION:
            {
                ios.close();
                return;
            }
            case WriteEngine::WE_SYNC_REQ_DATA:
            {
                onReceiveKeepAlive(ios, ibs);
                break;
            }
            case WriteEngine::WE_SYNC_RECIVE_DATA:
            {
                onReceiveKeepAlive(ios, ibs);
                break;
            }
            case WriteEngine::WE_SYNC_ROLLBACK:
            {
                onReceiveKeepAlive(ios, ibs);
                break;
            }
            case WriteEngine::WE_SYNC_SUCESS:
            {
                 onReceiveKeepAlive(ios, ibs);
                break;
            }
            case WriteEngine::WE_SYNC_RES_STATE:
            {
                 onReceiveKeepAlive(ios, ibs);
                break;
            }

            default:
                break;
        }
    };
    void SyncDataProcessor::msgProc(SyncClient * wc, messageqcpp::SBS &bs)
    {

    };

    SyncDataThread::SyncDataThread(SyncDataProcessor * syncProc,int PrgmID,uint16_t pmId):SyncClient((SyncBase * )syncProc,PrgmID,pmId) {
    
    };

    SyncDataThread::~SyncDataThread() {  };//~WEClient();

};
