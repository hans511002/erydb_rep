

#ifndef SYNC_WECLIENT_H__
#define SYNC_WECLIENT_H__

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
	class SyncClient;
	class SyncWEClient
	{
	public:
		EXPORT SyncWEClient(SyncClient * syncMgr, int PrgmID, uint16_t pmId);
		EXPORT ~SyncWEClient();
		EXPORT void write(const messageqcpp::ByteStream &msg);
		EXPORT int close();
		EXPORT void Setup();
	private:
		SyncWEClient(const SyncWEClient& weClient);
		void operator()();
		boost::thread * thReader;
		boost::shared_ptr<MessageQueueClient> client;
		//typedef std::map<unsigned, boost::shared_ptr<messageqcpp::MessageQueueClient> > ClientList;
		SyncClient * syncMgr;
		int fPrgmID;
		uint16_t fpmId;
		bool fBusy;
		bool isError;
		volatile uint32_t closingConnection;
		boost::mutex fOnErrMutex;   // to lock function scope to reset pmconnections under error condition
	};

	class SyncClient
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
		SyncClient();
		virtual void msgProc(SyncWEClient *, messageqcpp::SBS &bs);
		void closeWEClient(uint16_t pmId);
		void close();
	};
}

#undef EXPORT

#endif
// vim:ts=4 sw=4:
