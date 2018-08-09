/* Copyright (C) 2014 EryDB, Inc.
 * Copyright (C) 2016 MariaDB Corporation.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

// $Id: SyncClient.h   
/** @file */

#include <sstream>
#include <stdexcept>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <unistd.h>
#ifndef _MSC_VER
#include <arpa/inet.h>
#if __FreeBSD__
#include <sys/socket.h>
#endif
#endif
using namespace std;

#include <boost/thread/mutex.hpp>
using namespace boost;

#include "messagequeue.h"


#include "configcpp.h"
using namespace config;

#include "errorids.h"
#include "exceptclasses.h"
#include "messagelog.h"
#include "messageobj.h"
#include "loggingid.h"
using namespace logging;

#include "liboamcpp.h"
#include "alarmmanager.h"
using namespace alarmmanager;
using namespace oam;

#include "oamcache.h"
#include "SyncManager.h"
#include "we_messages.h"
using namespace WriteEngine;

#include "atomicops.h"


namespace SYNC
{
	void SyncManager::msgProc(SyncClient * wc, messageqcpp::SBS &bs)
	{

	};

	void SyncManager::addSyncData(SyncDataList &syncData)
	{};
	SYNC_DATA_STATE * SyncManager::setSyncDataState(SyncData &syncData, SYNC_STATE state)
	{};
	SYNC_DATA_STATE SyncManager::getSyncDataState(SyncData & syncData)
	{};
	SYNC_DATA_STATEList SyncManager::getAllSyncDataState(int state) {
	};
	SyncData * SyncManager::getNextSyncData()
	{};
	//master中使用,监听队列和子线程任务，发送同步数据命令 , 十分钟内无任务需要执行则退出
	void SyncManager::run() {
		while (this->running)
		{

		}
	};


}

// vim:ts=4 sw=4:
