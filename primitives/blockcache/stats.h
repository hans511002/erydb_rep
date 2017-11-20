/* Copyright (C) 2014 EryDB, Inc.

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

/*
* $Id: stats.h 2035 2013-01-21 14:12:19Z rdempsey $
*/

/** @file */

#ifndef PRIMPROC_STATS_H
#define PRIMPROC_STATS_H

#ifndef _MSC_VER
#include <pthread.h>
#else
//FIXME: find a portable solution
typedef int pthread_t;
#endif
#include <boost/thread.hpp>
#include <iostream>
#include <sstream>

#include "brm.h"
using namespace BRM;
namespace dbbc
{

class Stats
{
public:
	Stats();
	Stats(const char *name);
	virtual ~Stats();

	void touchedLBID(uint64_t lbid, pthread_t thdid, uint32_t session=0);
	void markEvent(const uint64_t lbid, const pthread_t thdid, const uint32_t session, const char event);

	inline BRM::OID_t lbid2oid(uint64_t lbid)
	{
		BRM::OID_t oid;
		DBROOTS_struct dbroot;
		uint32_t partNum;
		uint16_t segNum;
		uint32_t fbo;
		brm.lookupLocal(lbid, 0, false, oid, dbroot, partNum, segNum, fbo);
		return oid;
	}

private:
	Stats(const Stats& rhs);
	Stats& operator=(const Stats& rhs);

	boost::thread* fMonitorp;
	BRM::DBRM brm;
	//ostringstream fName;
	const char* fName;

};

}

#endif //PRIMPROC_STATS_H
