 

#ifndef WECLIENT_H__
#define WECLIENT_H__

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/scoped_array.hpp>

#include "bytestream.h"
//#include "we_message.h"
#include "threadsafequeue.h"
#include "rwlock_local.h"
#include "resourcemanager.h"
using namespace messageqcpp;
#include "syncData.h"

#if defined(_MSC_VER) && defined(xxxWEClient_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace SYNC {
class WEClient
{
public:
	EXPORT WEClient(SyncBase * syncMgr,int PrgmID,uint16_t pmId);
	EXPORT ~WEClient();
	EXPORT void write(const messageqcpp::ByteStream &msg);
	EXPORT int Close(); 
	EXPORT void Setup();
private:
	WEClient(const WEClient& weClient);
	void operator()();
    boost::thread * thReader;
    boost::shared_ptr<MessageQueueClient> client;
	typedef std::map<unsigned, boost::shared_ptr<messageqcpp::MessageQueueClient> > ClientList;
    SyncBase * syncMgr;
	int fPrgmID;
    uint16_t fpmId;
	bool fBusy;
	bool isError;
	volatile uint32_t closingConnection;
	boost::mutex fOnErrMutex;   // to lock function scope to reset pmconnections under error condition
};
}

#undef EXPORT

#endif
// vim:ts=4 sw=4:
