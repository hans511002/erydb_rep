#include "we_messages.h"
#include "syncDataProcessor.h"

namespace sync
{
    SyncDataProcessor::SyncDataProcessor() {

    };
    void SyncDataProcessor::onReceiveKeepAlive(const messageqcpp::IOSocket& ios,ByteStream& Ibs){

    };
    void SyncDataProcessor::msgProc(ByteStream::byte msgId,const messageqcpp::IOSocket& ios, messageqcpp::ByteStream &ibs) {
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
    void SyncDataProcessor::msgProc(WEClient * wc, messageqcpp::SBS &bs)
    {

    };

    SyncDataThread(SyncDataProcessor * syncProc, messageqcpp::IOSocket *sock) {
    
    };


};
