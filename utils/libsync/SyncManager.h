 

#ifndef SYNC_MANAGER_H__
#define SYNC_MANAGER_H__

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
#include "SyncClient.h"

#if defined(_MSC_VER) && defined(xxxWEClient_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace SYNC {
	//master内同步管理
	class SyncManager  :public SyncClient
	{
	private: 
		SyncManager() : curIndex(-1) {
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
		SYNC_DATA_STATEList getAllSyncDataState(int state = 0);
		SyncData * getNextSyncData();
		virtual void msgProc(SyncClient *, messageqcpp::SBS &);

		void run();
		uint16_t connectedWEServers() const { return syncThreads.size(); }
	};
	extern SyncManager * syncMgr;
}

#undef EXPORT

#endif
// vim:ts=4 sw=4:
