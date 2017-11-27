/* Copyright (C) 2014 EryDB, Inc.
   Copyright (C) 2016 EryDB, Inc.

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

#ifndef OAMCACHE_H_
#define OAMCACHE_H_

#include <unistd.h>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "liboamcpp.h"

#if defined(_MSC_VER) && defined(xxxOAMCACHE_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace oam
{

class OamCache {
public:
	typedef boost::shared_ptr<std::map<uint16_t, uint16_t> > IntIntMap;
	typedef std::vector<uint16_t> dbRoots;
	typedef boost::shared_ptr<std::map<uint16_t, dbRoots> > IntListIntMap;
	EXPORT virtual ~OamCache();

	EXPORT void checkReload();
	EXPORT void forceReload() {mtime=0;}

	EXPORT IntIntMap getDBRootToPMMap();
	EXPORT IntIntMap getDBRootToConnectionMap();
	EXPORT IntListIntMap getPMToDbrootsMap();
	EXPORT uint32_t getDBRootCount();
    EXPORT unsigned getPMCount();
	EXPORT DBRootConfigList& getDBRootNums();
    EXPORT std::vector<uint16_t> & getDbrootList(uint16_t pm=0);//pm =0 localpm
    EXPORT bool existDbroot(uint16_t dbr, uint16_t pm = 0);
	EXPORT std::vector<uint16_t> & getModuleIds();
	EXPORT static OamCache * makeOamCache();
    EXPORT std::string getOAMParentModuleName();
    EXPORT int getOAMParentModuleId();
	EXPORT int getLocalPMId();	// return the PM id of this machine.
	EXPORT std::string getSystemName();
	EXPORT std::string getModuleName();
     
private:
	OamCache();
	OamCache(const OamCache &);
	// OamCache & operator=(const OamCache &) const;

    IntIntMap dbRootPMMap;
    IntIntMap dbRootConnectionMap;
    IntListIntMap pmDbrootsMap;
    IntIntMap localDbrootMap;

	uint32_t numDBRoots;
	time_t mtime;
	DBRootConfigList dbroots;
	std::vector<uint16_t> moduleIds;
	std::string OAMParentModuleName;
    uint16_t OAMParentModuleId;
    uint16_t mLocalPMId;	// The PM id running on this machine
	std::string systemName;
	std::string moduleName;
    //localModule, localModuleType, localModuleID, ParentOAMModule, parentOAMModuleFlag, serverTypeInstall, StandbyOAMModule, standbyOAMModuleFlag
    oamModuleInfo_t oamModInfo;
 
  
};

} /* namespace */

#undef EXPORT

#endif /* OAMCACHE_H_ */
