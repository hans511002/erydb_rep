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

#include <string>
//#define NDEBUG
#include <cassert>
#include <map>
#include <set>
using namespace std;

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
using namespace boost;

#define OAMCACHE_DLLEXPORT
#include "oamcache.h"
#undef OAMCACHE_DLLEXPORT
#include "liboamcpp.h"
#include "exceptclasses.h"
#include "configcpp.h"
#include "installdir.h"

namespace
{
	oam::OamCache *oamCache = 0;
	mutex cacheLock;
}

namespace oam
{

OamCache * OamCache::makeOamCache()
{
	mutex::scoped_lock lk(cacheLock);
	if (oamCache == 0)
		oamCache = new OamCache();
	return oamCache;
}

OamCache::OamCache() : mtime(0), mLocalPMId(0){
	string localModule;
	string moduleType;
	string fileName = startup::StartUp::installDir() + "/local/module";
	ifstream moduleFile (fileName.c_str());
	char line[400];
	while (moduleFile.getline(line, 400))
	{
		localModule = line;
		break;
	}
	moduleFile.close();
	mLocalPMId = 0;
	if (!localModule.empty() ) 
	{
    	moduleType = localModule.substr(0,MAX_MODULE_TYPE_SIZE);
    	mLocalPMId = atoi(localModule.substr(MAX_MODULE_TYPE_SIZE,MAX_MODULE_ID_SIZE).c_str());
    	if (moduleType != "pm")
    	{
    		mLocalPMId = 0;
    	}
    }
}

OamCache::~OamCache()
{}

void OamCache::checkReload()
{
	Oam oam;
	config::Config *config = config::Config::makeConfig();
	int temp;

	if (config->getCurrentMTime() == mtime)
		return;
	dbroots.clear();
	oam.getSystemDbrootConfig(dbroots);

	string txt = config->getConfig("SystemConfig", "DBRootCount");
	mtime = config->getLastMTime();   // get 'last' after the first access; quicker than 'current'
	erydbassert(txt != "");
	numDBRoots = config->fromText(txt);

    dbRootPMMap.reset(new map<uint16_t, uint16_t>());
    localDbrootMap.reset(new map<uint16_t, uint16_t>());
    
	//cout << "reloading oamcache\n";	
	for (uint32_t i = 0; i < dbroots.size(); i++) {
		oam.getDbrootPmConfig(dbroots[i], temp);
		//cout << "  dbroot " << dbroots[i] << " -> PM " << temp << endl;
		(*dbRootPMMap)[dbroots[i]] = temp;
	}

	ModuleTypeConfig moduletypeconfig; 
	std::set<uint16_t> uniquePids;
	oam.getSystemConfig("pm", moduletypeconfig);
	int moduleID = 0;
	for (unsigned i = 0; i < moduletypeconfig.ModuleCount; i++) {
        if ((moduletypeconfig.ModuleNetworkList[i]).DeviceName.empty()){
            ostringstream oss;
            oss << "OamCache::checkReload(): modle ModuleNetworkList[" << i<<"] DeviceName is null";
            BRM::log_errno(oss.str() );
            throw runtime_error(oss.str());
        }
		moduleID = atoi((moduletypeconfig.ModuleNetworkList[i]).DeviceName.substr(MAX_MODULE_TYPE_SIZE,MAX_MODULE_ID_SIZE).c_str());
		uniquePids.insert(moduleID); 
	}
	std::set<uint16_t>::const_iterator it = uniquePids.begin();
	moduleIds.clear();
	uint32_t i = 0;
	map<uint16_t, uint16_t> pmToConnectionMap;
#ifdef _MSC_VER
	moduleIds.push_back(*it);
	pmToConnectionMap[*it] = i++;
#else
    // Restore for Windows when we support multiple PMs
	while (it != uniquePids.end())
	{
#if  !defined(SKIP_OAM_INIT)
		{
			try {
				int state = oam::MAN_INIT; 
				bool degraded;
				char num[80];
				int retry = 0;
				// MCOL-259 retry for 5 seconds if the PM is in some INIT mode.
				while ((   state == oam::BUSY_INIT 
						|| state == oam::MAN_INIT 
					    || state == oam::PID_UPDATE)
				       && retry < 5)
				{
					snprintf(num, 80, "%d", *it);
					try {
						oam.getModuleStatus(string("pm") + num, state, degraded);
					}
					catch (std::exception& e)
					{
						ostringstream os;
						os << "OamCache::checkReload exception while getModuleStatus pm" << num << " " << e.what();
						oam.writeLog(os.str(), logging::LOG_TYPE_ERROR);
						break;
					}
					catch (...) {
						ostringstream os;
						os << "OamCache::checkReload exception while getModuleStatus pm" << num;
						oam.writeLog(os.str(), logging::LOG_TYPE_ERROR);
						break;
					}

					if (state == oam::ACTIVE || state == oam::DEGRADED) {
						pmToConnectionMap[*it] = i++;
						moduleIds.push_back(*it);
						break;
					}
					sleep(1);
				//cout << "pm " << *it << " -> connection " << (i-1) << endl;
				}
				if (state != oam::ACTIVE)
				{
					ostringstream os;
					os << "OamCache::checkReload shows state for pm" << num << " as " << oamState[state];
					oam.writeLog(os.str(), logging::LOG_TYPE_ERROR);
				}
			}
			catch (std::exception& e)
			{
				ostringstream os;
				os << "OamCache::checkReload final exception while getModuleStatus " << e.what();
				oam.writeLog(os.str(), logging::LOG_TYPE_ERROR);
				break;
			}
			catch (...) {
				ostringstream os;
				os << "OamCache::checkReload final exception while getModuleStatus";
				oam.writeLog(os.str(), logging::LOG_TYPE_ERROR);
				break;
			}
		}
#else
		moduleIds.push_back(*it);
#endif
		it++;
		
	}
#endif
	dbRootConnectionMap.reset(new map<uint16_t, uint16_t>());
	for (i = 0; i < dbroots.size(); i++)
	{
		map<uint16_t, uint16_t>::iterator pmIter = pmToConnectionMap.find((*dbRootPMMap)[dbroots[i]]);
		if (pmIter != pmToConnectionMap.end())
		{
            uint16_t pmId = (*pmIter).first;
			(*dbRootConnectionMap)[dbroots[i]] = (*pmIter).second;
            if (pmId== mLocalPMId)
            {
                (*localDbrootMap)[mLocalPMId] = pmId;
            }
		}
	}

	pmDbrootsMap.reset(new OamCache::UintListUintMap::element_type());
	systemStorageInfo_t t;
	t = oam.getStorageConfig();
	DeviceDBRootList moduledbrootlist = boost::get<2>(t);
	DeviceDBRootList::iterator pt = moduledbrootlist.begin();
	for( ; pt != moduledbrootlist.end() ; pt++)
	{
		moduleID = (*pt).DeviceID;
		DBRootConfigList::iterator pt1 = (*pt).dbrootConfigList.begin();
		for( ; pt1 != (*pt).dbrootConfigList.end(); pt1++)
		{
			(*pmDbrootsMap)[moduleID].push_back(*pt1);
		}
	}
	
    oamModInfo = oam.getModuleInfo();
    //localModule, localModuleType, localModuleID, ParentOAMModule, parentOAMModuleFlag, serverTypeInstall, StandbyOAMModule, standbyOAMModuleFlag
    OAMParentModuleName = boost::get<3>(oamModInfo);
	systemName = config->getConfig("SystemConfig", "SystemName");
    string moduleType = OAMParentModuleName.substr(0, MAX_MODULE_TYPE_SIZE);
    OAMParentModuleId = atoi(OAMParentModuleName.substr(MAX_MODULE_TYPE_SIZE, MAX_MODULE_ID_SIZE).c_str());
    if (moduleType != "pm") {
        OAMParentModuleId = 0;
    }
}

OamCache::UintUintMap OamCache::getDBRootToPMMap()
{
	mutex::scoped_lock lk(cacheLock);
	checkReload();
	return dbRootPMMap;
}

OamCache::UintUintMap OamCache::getDBRootToConnectionMap()
{
	mutex::scoped_lock lk(cacheLock);
	checkReload();
	return dbRootConnectionMap;
}

OamCache::UintListUintMap OamCache::getPMToDbrootsMap()
{
	mutex::scoped_lock lk(cacheLock);
	checkReload();
	return pmDbrootsMap;
}

uint32_t OamCache::getDBRootCount()
{
	mutex::scoped_lock lk(cacheLock);
	checkReload();
	return numDBRoots;
}

DBRootConfigList& OamCache::getDBRootNums()
{ 
	mutex::scoped_lock lk(cacheLock);
	checkReload();
	return dbroots; 
}

std::vector<uint16_t>& OamCache::getModuleIds()
{
	mutex::scoped_lock lk(cacheLock);
	checkReload();
	return moduleIds; 
}

std::string OamCache::getOAMParentModuleName()
{
	mutex::scoped_lock lk(cacheLock);
	checkReload();
	return OAMParentModuleName; 
}
int OamCache::getOAMParentModuleId() {
    mutex::scoped_lock lk(cacheLock);
    checkReload();
    return OAMParentModuleId;
}

int OamCache::getLocalPMId()
{
	// mutex::scoped_lock lk(cacheLock);
	// This comes from the file $INSTALL/local/module, not from the xml.
	// Thus, it's not refreshed during checkReload(). 
	return mLocalPMId;
}

string OamCache::getSystemName()
{
	mutex::scoped_lock lk(cacheLock);

	checkReload();
	return systemName; 
}

string OamCache::getModuleName()
{
	mutex::scoped_lock lk(cacheLock);

	if (!moduleName.empty())
		return moduleName;

	string fileName = startup::StartUp::installDir() + "/local/module";
	ifstream moduleFile(fileName.c_str());
	getline(moduleFile, moduleName);
	moduleFile.close();

	return moduleName; 
}
unsigned  OamCache::getRepSize() {
    BRM::ExtentMap em;
    return em.getRepSize();
};

unsigned OamCache::getPMCount() {
    mutex::scoped_lock lk(cacheLock);
    checkReload();
    return this->moduleIds.size();
} 
std::vector<uint16_t> & OamCache::getDbrootList(uint16_t pm) {//pm =0 localpm
    mutex::scoped_lock lk(cacheLock);
    checkReload();
    if (pm == 0) 
        pm = mLocalPMId;  
    UintListUintMap::element_type::iterator it= pmDbrootsMap->find(pm);
    if (it != pmDbrootsMap->end()) 
        return it->second;
    ostringstream oss;
    oss << "OamCache::getDbrootList: pm not found: "<<pm ;
    BRM::log(oss.str(), logging::LOG_TYPE_ERROR);
    throw logic_error(oss.str());
};
bool OamCache::existDbroot(uint16_t dbr, uint16_t pm  ) {
    mutex::scoped_lock lk(cacheLock);
    checkReload();
    if (pm == 0)
        pm = mLocalPMId;
    if (pm == mLocalPMId)
       return localDbrootMap->find(dbr) != localDbrootMap->end();
    std::vector<uint16_t> pmDbrlist=  getDbrootList(pm);
    for (std::vector<uint16_t>::iterator it = pmDbrlist.begin(); it != pmDbrlist.end(); it++){
        if (*it == dbr)
            return true;
    }
    return false;
};
std::vector<uint16_t> OamCache::getDBrootPms(const BRM::DBROOTS_struct &dbRoot) {
    int pmCount = this->getPMCount();
    std::vector<uint16_t> pmIds;
    UintUintMap dbRootPMMap = getDBRootToPMMap();
    if (pmCount > 1)
    {
        for (int i = 0; i < MAX_DATA_REPLICATESIZE; i++)
        {
            uint16_t dbr = dbRoot.dbRoots[i];
            if (dbr > 0)
            {
                uint16_t pmid = (*dbRootPMMap)[dbr];
                if(pmid)pmIds.push_back(pmid);
            } else
            {
                break;
            }
        }
    } else
    {
        UintUintMap pmMap;
        pmMap.reset(new UintUintMap::element_type());
        for (int i = 0; i < MAX_DATA_REPLICATESIZE; i++)
        {
            uint16_t dbr = dbRoot.dbRoots[i];
            if (dbr > 0)
            {
                uint16_t pmid = (*dbRootPMMap)[dbr];
                if (pmMap->find(pmid) == pmMap->end())
                {
                    (*pmMap)[pmid] = pmIds.size();
                    if(pmid)pmIds.push_back(pmid);
                }
            } else
            {
                break;
            }
        }
    }
    return pmIds;
};

} /* namespace oam */
