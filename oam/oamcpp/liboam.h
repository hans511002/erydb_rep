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

/******************************************************************************************
 ******************************************************************************************/
/**
 * @file
 */
#ifndef LIBOAM_H
#define LIBOAM_H

#include <exception>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits.h>
#include <sstream>
#include <vector>
#ifdef __linux__
#include <sys/sysinfo.h>
#include <netdb.h>
#endif
#include <fcntl.h>

#include "boost/tuple/tuple.hpp"

#if defined(_MSC_VER) && defined(xxxLIBOAM_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace oam
{

    /*
     * 	Global OAM parmaters
     */

    /** @brief Maximum Number of Modules within the erydb System
     */
    const int MAX_MODULE = 1024;

    /** @brief Maximum Number of DBRoots within the erydb System
     */
    const int MAX_DBROOT = 10240;
    const int MAX_DBROOT_AMAZON = 190;	//DUE TO DEVICE NAME LIMIT
 
    /** @brief default and min first user object id 
    */
    const int USER_OBJECT_ID = MAX_DBROOT + 2000;

   /** @brief Maximum Number of Modules Types within the erydb System
     */
    const int MAX_MODULE_TYPE = 3;

    /** @brief Maximum Number of External Devices within the erydb System
     */
    const int MAX_EXT_DEVICE = 20;

    /** @brief Maximum Number of Arguments per process
     */
    const int MAX_ARGUMENTS = 15;

    /** @brief Maximum Number of Dependancy processes per process
     */
    const int MAX_DEPENDANCY = 6;

    /** @brief Maximum Number of processes within the erydb System
     */
    const int MAX_PROCESS_PER_MODULE = 15;
    const int MAX_PROCESS = MAX_MODULE*MAX_PROCESS_PER_MODULE;

    /** @brief Maximum Number of Parameters per process
     */
    const int MAX_PARAMS = 13;

    /** @brief Maximum Module Type Size
     */
    const int MAX_MODULE_TYPE_SIZE = 2;

    /** @brief Maximum Module ID Size
     */
    const int MAX_MODULE_ID_SIZE = 4;

    /** @brief Maximum Number of NICs per Module
     */
    const int MAX_NIC = 4;

    /** @brief Unassigned Name and IP Address Value
     */
    const std::string UnassignedIpAddr = "0.0.0.0";
    const std::string UnassignedName = "unassigned";


    /** @brief erydb System Configuration file sections
     */
    const std::string configSections[] = {	"SystemConfig",
											"SystemModuleConfig",
											"SystemExtDeviceConfig",
											"SessionManager",
											"VersionBuffer",
											"OIDManager",
											"PrimitiveServers",
											"Installation",
											"ExtentMap",
											"MessageLog",
											""
	};

    /** @brief gluster control commands
     */
    enum GLUSTER_COMMANDS
    {
        GLUSTER_STATUS,
        GLUSTER_SETDDC,
        GLUSTER_ASSIGN,
        GLUSTER_WHOHAS,
        GLUSTER_UNASSIGN,
        GLUSTER_ADD,
        GLUSTER_DELETE
	};


    /** @brief mysql-erydb Action
     */
    enum MYSQLERYDB_ACTION
    {
        MYSQL_START,
        MYSQL_STOP,
        MYSQL_RESTART,
        MYSQL_RELOAD,
		MYSQL_FORCE_RELOAD,
		MYSQL_STATUS
	};

    /** @brief Device Notification Type
     */
    enum NOTIFICATION_TYPE
    {
        NOTIFICATION_TYPE_RESERVED,  // 0 = not used
		START_PM_MASTER_DOWN,
		START_PM_STANDBY_DOWN,
		START_PM_COLD_DOWN,
		START_UM_DOWN,
		MODULE_DOWN,
		START_STANDBY_TO_MASTER,
		PM_MASTER_ACTIVE,
		PM_STANDBY_ACTIVE,
		PM_COLD_ACTIVE,
		UM_ACTIVE,
		PM_MASTER_FAILED_DISABLED,
		DBROOT_DOWN,
		DBROOT_UP,
		DB_HEALTH_CHECK_FAILED,
		DBROOT_MOUNT_FAILURE,
		MODULE_UP
    };

	const uint32_t NOTIFICATIONKEY = 0x49444231;

    /** @brief Server Type Installs
     */

    enum INSTALLTYPE
    {
        RESERVED,                              		// 0 = not used
        INSTALL_NORMAL,                       		// 1 = Normal - dm/um/pm on a seperate servers
        INSTALL_COMBINE_DM_UM_PM,                   // 2 = dm/um/pm on a single server
        INSTALL_COMBINE_DM_UM,                      // 3 = dm/um on a same server
        INSTALL_COMBINE_PM_UM                       // 4 = pm/um on a same server
    };

    /** @brief Server Monitor Message Request options
     */

    enum SERVERMONITOR_TYPE_REQUEST
    {
		GET_PROC_CPU_USAGE,
		GET_MODULE_CPU_USAGE,
		GET_PROC_MEMORY_USAGE,
		GET_MODULE_MEMORY_USAGE,
		GET_MODULE_DISK_USAGE,
		GET_ACTIVE_SQL_QUERY,
		RUN_DBHEALTH_CHECK
    };


    /** @brief OAM API Return values
     */

    enum API_STATUS
    {
        API_SUCCESS,
        API_FAILURE,
        API_INVALID_PARAMETER,
        API_FILE_OPEN_ERROR,
        API_TIMEOUT,
        API_DISABLED,
		API_FILE_ALREADY_EXIST,
		API_ALREADY_IN_PROGRESS,
        API_MINOR_FAILURE,
		API_FAILURE_DB_ERROR,
		API_INVALID_STATE,
		API_READONLY_PARAMETER,
		API_TRANSACTIONS_COMPLETE,
		API_CONN_REFUSED,
        API_CANCELLED,
        API_STILL_WORKING,
        API_MAX
    };

    /** @brief OAM Parent Module Indicator
     */

    enum OAM_MASTER_MODULE
    {
        PARENT_NO,
        PARENT_YES
    };

    /** @brief Realtime Linux OS Module Indicator
     */

/*    enum RT_LINUX_Module
    {
        RT_LINUX_NO,
        RT_LINUX_YES
    };
*/
    /** @brief Process and Hardware States
     */

    enum STATE
    {
        MAN_OFFLINE,                              // 0 = Manual disable mode
        AUTO_OFFLINE,                             // 1 = Auto disable, due to a fault
        MAN_INIT,                                 // 2 = Manual initialization mode
        AUTO_INIT,                                // 3 = Auto initialization mode
        ACTIVE,                                   // 4 = Active mode
        LEAVE_BLANK,							  // when this was standby, 'PuTTY' would show up in the console
        STANDBY,                                  // 6 = Hot Standby mode
        FAILED,                                   // 7 = Failed restoral mode
        UP,                                       // 8 = Up mode, for hardware devices
        DOWN,                                     // 9 = Down mode, for hardware devices
        COLD_STANDBY,                             // 10 = Cold Standby mode
        UNEQUIP,                                  // 11 = Unequipped mode
        EQUIP,                                    // 12 = Equipped mode
        DEGRADED,                                 // 13 = Degraded mode
        MAN_DISABLED,                             // 14 = Manual Disabled mode
        AUTO_DISABLED,                            // 15 = Auto Disabled mode
        ENABLED,                                  // 16 = Enabled mode
        INITIAL,                                  // 17 = Initial mode
		STANDBY_INIT,							  // 18 = Standby init
		BUSY_INIT, 							  	  // 19 = Busy init
		ROLLBACK_INIT,  					  	  // 20 = Rollback during DML init
		PID_UPDATE,  					  	      // 21 = Assigning the pid
		STATE_MAX								  // 22 = Max value
    };

    /** @brief String State
     */
    const std::string oamState[] =
    {
		"MAN_OFFLINE",
    	"AUTO_OFFLINE",
    	"MAN_INIT",
    	"AUTO_INIT",
    	"ACTIVE",
		"LEAVE_BLANK",
    	"HOT_STANDBY",
    	"FAILED",
    	"UP",
    	"DOWN",
    	"COLD_STANDBY",
        "UNEQUIP",
        "EQUIP",
    	"DEGRADED",
    	"MAN_DISABLED",
    	"AUTO_DISABLED",
    	"ENABLED",
    	"INITIAL",
    	"STANDBY_INIT",
    	"BUSY_INIT",
		"ROLLBACK_INIT",
		"PID_UPDATE"
        ""
    };
    /** @brief Process and Hardware String States
     */

    const std::string MANOFFLINE = "MAN_OFFLINE";
    const std::string AUTOOFFLINE = "AUTO_OFFLINE";
    const std::string MANINIT = "MAN_INIT";
    const std::string AUTOINIT = "AUTO_INIT";
    const std::string ACTIVESTATE = "ACTIVE";
    const std::string STANDBYSTATE = "HOT_STANDBY";
    const std::string FAILEDSTATE = "FAILED";
    const std::string UPSTATE = "UP";
    const std::string DOWNSTATE = "DOWN";
    const std::string COLDSTANDBYSTATE = "COLD_STANDBY";
    const std::string INITIALSTATE = "INITIAL";
    const std::string DEGRADEDSTATE = "DEGRADED";
    const std::string ENABLEDSTATE = "ENABLED";
    const std::string MANDISABLEDSTATE = "MAN_DISABLED";
    const std::string AUTODISABLEDSTATE = "AUTO_DISABLED";
    const std::string STANDBYINIT = "STANDBY_INIT";
    const std::string BUSYINIT = "BUSY_INIT";

    /** @brief Module/Process Run Types
     */

    const std::string ACTIVE_STANDBY = "ACTIVE_STANDBY";
    const std::string LOADSHARE = "LOADSHARE";
    const std::string BROADCAST = "BROADCAST";
    const std::string SIMPLEX = "SIMPLEX";

    /** @brief Module Equippage states
     */

    const std::string EQUIP_YES = "EQ";
    const std::string EQUIP_NO = "NE";


    /** @brief Update Logging Levels
     */
    const std::string LogLevel[] =
    {
        "critical",
        "error",
        "warning",
        "info",
        "debug",
        "all",
        ""
    };

    /** @brief Logging Level file name
     *
     * NOTE: make sure this list is insync with above LogLevel list
     */
    const std::string LogFile[] =
    {
        "local1.=crit -/var/log/erydb/crit.log",
        "local1.=err -/var/log/erydb/err.log",
        "local1.=warning -/var/log/erydb/warning.log",
        "local1.=info -/var/log/erydb/info.log",
        "local1.=debug -/var/log/erydb/debug.log",
        ""
    };

    const std::string LogFile7[] =
    {
        "local1.crit -/var/log/erydb/crit.log",
        "local1.err -/var/log/erydb/err.log",
        "local1.warning -/var/log/erydb/warning.log",
        "local1.info -/var/log/erydb/info.log",
        "local1.debug -/var/log/erydb/debug.log",
        ""
    };

    /** @brief Log Config Data map
     */
    typedef struct LogConfigData_struct
    {
        std::string     moduleName;
        int             configData;
    }
    LogConfigData;

    typedef std::vector<LogConfigData> SystemLogConfigData;

    /** @brief LogConfigData level bitmap
     */
    enum LEVEL_FLAGS
    {
        LEVEL_CRITICAL = 0x1,
        LEVEL_ERROR = 0x2,
        LEVEL_WARNING = 0x4,
        LEVEL_INFO = 0x8,
        LEVEL_DEBUG = 0x10,
    };

    /** @brief Alarm IDs
     */

    enum ALARMS
    {
        ALARM_NONE,                                 // 0 = NO ALARM
        CPU_USAGE_HIGH,                           // 1 = CPU Usage High threahold crossed
        CPU_USAGE_MED,                            // 2 = CPU Usage Medium threshold crossed
        CPU_USAGE_LOW,                            // 3 = CPU Usage Low threashold crossed
        DISK_USAGE_HIGH,                          // 4 = DISK Usage High threahold crossed
        DISK_USAGE_MED,                           // 5 = DISK Usage Medium threshold crossed
        DISK_USAGE_LOW,                           // 6 = DISK Usage Low threashold crossed
        MEMORY_USAGE_HIGH,                        // 7 = MEMORY Usage High threahold crossed
        MEMORY_USAGE_MED,                         // 8 = MEMORY Usage Medium threshold crossed
        MEMORY_USAGE_LOW,                         // 9 = MEMORY Usage Low threashold crossed
        SWAP_USAGE_HIGH,                          // 10 = SWAP Usage High threahold crossed
        SWAP_USAGE_MED,                           // 11 = SWAP Usage Medium threshold crossed
        SWAP_USAGE_LOW,                           // 12 = SWAP Usage Low threashold crossed
        PROCESS_DOWN_AUTO,                        // 13 = Process is down due to fault
        MODULE_DOWN_AUTO,                         // 14 = Module is down due to fault
        SYSTEM_DOWN_AUTO,                         // 15 = System is down due to fault
        POWERON_TEST_SEVERE,                      // 16 = Power-On test Module Warning error
        POWERON_TEST_WARNING,                     // 17 = Power-On test Warning error
        HARDWARE_HIGH,                            // 18 = Hardware Critical alarm
        HARDWARE_MED,                             // 19 = Hardware Major alarm
        HARDWARE_LOW,                             // 20 = Hardware Minor alarm
        PROCESS_DOWN_MANUAL,                      // 21 = Process is down due to operator request
        MODULE_DOWN_MANUAL,                       // 22 = Module is down due to operator request
        SYSTEM_DOWN_MANUAL,                       // 23 = System is down due to operator request
        EXT_DEVICE_DOWN_AUTO,                     // 24 = External Device is down due to fault
        PROCESS_INIT_FAILURE,                     // 25 = Process Initization Failure
        NIC_DOWN_AUTO,                            // 26 = NIC is down due to fault
        DBRM_LOAD_DATA_ERROR,                     // 27 = DBRM Load Data error
        INVALID_SW_VERSION,                       // 28 = Invalid Software Version
        STARTUP_DIAGNOTICS_FAILURE,               // 29 = Module Startup Dianostics Failure
        CONN_FAILURE,                             // 30 = Connect Failure
        DBRM_READ_ONLY,                           // 31 = The DBRM is read-only
        EE_LICENSE_EXPIRED,                       // 32 = Enterprise License has expired
        MODULE_SWITCH_ACTIVE,                     // 33 = PM Failover / Switchover
        ROLLBACK_FAILURE,                     	  // 34 = DB Rollback Failure
        GLUSTER_DISK_FAILURE,                     // 35 = Gluster Disk Copy Failure
		INVALID_LOCALE,                           // 36 = Locale invalid
        MAX_ALARM_ID
    };

    /** @brief Alarm Severity
     */

    enum ALARM_SEVERITY
    {
        NO_SEVERITY,                              // 0 = N/A
        CRITICAL,                                 // 1 = CRITICAL
        MAJOR,                                    // 2 = MAJOR
        MINOR,                                    // 3 = MINOR
        WARNING,                                  // 4 = WARNING
        INFORMATIONAL                             // 5 = INFORMATIONAL
    };

    /** @brief OAM Hardware Management User Authorization level
     */

    enum AUTH_LEVEL
    {
        ADMINISTRATION,                           // 0 = Admin Level
        MAINTENANCE                               // 1 = Maintenance Level
    };

    /** @brief Boot Launch flag
     */

    enum LAUNCH_FLAG
    {
        INIT_LAUNCH,                              // 0 = Process launched by OS Init
        BOOT_LAUNCH,                              // 1 = Process launched by ProcMon at boot time
        MGR_LAUNCH                                // 2 = Process lanuched by ProcMgr after System reboot
    };

    /** @brief Process Management API request options
     *
     * 		Message from a UI to Process Manager
     */

    enum PROC_MGT_MSG_REQUEST
    {
        STOPMODULE,
        STARTMODULE,
        RESTARTMODULE,
        ENABLEMODULE,
        DISABLEMODULE,
        STARTSYSTEM,
        STOPSYSTEM,
        RESTARTSYSTEM,
        SHUTDOWNMODULE,
        SHUTDOWNSYSTEM,
        STOPPROCESS,
        STARTPROCESS,
        RESTARTPROCESS,
        UPDATELOG,
        GETCONFIGLOG,
        REINITPROCESS,
        UPDATECONFIG,
	BUILDSYSTEMTABLES,
        ADDMODULE,
        REMOVEMODULE,
        RECONFIGUREMODULE,
        STOPPROCESSTYPE,
        STARTPROCESSTYPE,
        RESTARTPROCESSTYPE,
        REINITPROCESSTYPE,
	DISTRIBUTECONFIG,
	SWITCHOAMPARENT,
	UNMOUNT,
	MOUNT,
        SUSPENDWRITES,
	FSTABUPDATE,
	ENABLEMYSQLREP,
	DISABLEMYSQLREP,
	GLUSTERASSIGN,
	GLUSTERUNASSIGN,
		PROCESSALARM
    };

    /** @brief Process Management - Mgr to Mon request options
     *
     * 		Message from a Process Manager to Process Monitor
     */

    enum PROC_MGR_MSG_REQUEST
    {
        STOPALL,
        STOP,
        START,
        RESTART,
        STARTALL,
        PROCREINITPROCESS
    };

    /** @brief Process Management API type options
     *
     *   	Message from Process Manager to Process Monitor
     *		Process Monitor to Manager to UI
     */

    enum PROC_MGT_TYPE_REQUEST
    {
        REQUEST,
        ACK,
        REPORT_STATUS,
        PROCUPDATELOG,
        PROCGETCONFIGLOG,
        CHECKPOWERON,
	PROCUPDATECONFIG,
        HEARTBEAT_REGISTER,
        HEARTBEAT_DEREGISTER,
        HEARTBEAT_SEND,
	PROCBUILDSYSTEMTABLES,
	LOCALHEARTBEAT,
	RECONFIGURE,
	PROCESSRESTART,
	GETSOFTWAREINFO,
	UPDATEEXPORTS,
	UPDATEPARENTNFS,
	OAMPARENTACTIVE,
	UPDATECONFIGFILE,
	GETDBRMDATA,
	GETPARENTOAMMODULE,
	OAMPARENTCOLD,
	GETALARMDATA,
	GETACTIVEALARMDATA,
	RUNUPGRADE,
	PROCUNMOUNT,
	PROCMOUNT,
	PROCFSTABUPDATE,
	MASTERREP,
	SLAVEREP,
	MASTERDIST,
	DISABLEREP,
	PROCGLUSTERASSIGN,
	PROCGLUSTERUNASSIGN
    };


    /** @brief Hardware and process shutdown flag
     */

    enum GRACEFUL_FLAG
    {
        GRACEFUL,
        FORCEFUL,
		INSTALL,
		REMOVE,
		GRACEFUL_STANDBY,
		STATUS_UPDATE,
        GRACEFUL_WAIT            // Wait for all table locks and transactions to finish.
    };

    /** @brief Acknowledgment indication flag
     */

    enum ACK_FLAG
    {
        ACK_NO,
        ACK_YES
    };

    /** @brief Responses to cancel/wait/rollback/force question
     *  
     *  When a suspend, stop, restart or shutdown of system is
     *  requested, the user is asked this question.
      */
    enum CC_SUSPEND_ANSWER
    {
        CANCEL,
        WAIT,
        ROLLBACK,
        FORCE
    };

    /** @brief Process Management Status Request types
      */

    enum STATUS_TYPE_REQUEST
    {
		GET_PROC_STATUS,
		SET_PROC_STATUS,
		GET_ALL_PROC_STATUS,
		GET_PROC_STATUS_BY_PID,
		GET_SYSTEM_STATUS,
		SET_SYSTEM_STATUS,
		SET_MODULE_STATUS,
		SET_EXT_DEVICE_STATUS,
		ADD_MODULE,
		REMOVE_MODULE,
		RECONFIGURE_MODULE,
		SET_NIC_STATUS,
		SET_PM_IPS,
		ADD_EXT_DEVICE,
		REMOVE_EXT_DEVICE,
		GET_SHARED_MEM,
		SET_DBROOT_STATUS,
		ADD_DBROOT,
		REMOVE_DBROOT
    };

    /** @brief System Software Package Structure
     *
     *   Structure that is returned by the getSystemSoftware API
     */

    struct SystemSoftware_s
    {
        std::string Version;                      //!< System Software Version
        std::string Release;                      //!< System Software Release
    };
    typedef struct SystemSoftware_s SystemSoftware;

    /** @brief System Software Package parse data
     */
    const std::string SoftwareData[] =
    {
        "version=",
        "release=",
        ""
    };

    /** @brief System Configuration Structure
     *
     *   Structure that is returned by the getSystemConfigFile API for the
     *   System Configuration data stored in the System Configuration file
     */

    struct SystemConfig_s
    {
        std::string SystemName;                    //!< System Name
        int32_t ModuleHeartbeatPeriod;           //!< Module Heartbeat period in minutes
        uint32_t ModuleHeartbeatCount;            //!< Module Heartbeat failure count
//        int32_t ProcessHeartbeatPeriod;          //!< Process Heartbeat period in minutes
        std::string NMSIPAddr;                    //!< NMS system IP address
        std::string DNSIPAddr;                    //!< DNS IP address
        std::string LDAPIPAddr;                   //!< LDAP IP address
        std::string NTPIPAddr;                    //!< NTP IP address
        uint32_t DBRootCount;                  		//!< Database Root directory Count
		std::vector<std::string> DBRoot;			//!< Database Root directories
        std::string DBRMRoot;                     //!< DBRM Root directory
        uint32_t ExternalCriticalThreshold;     	  //!< External Disk Critical Threahold %
        uint32_t ExternalMajorThreshold;        	  //!< External Disk Major Threahold %
        uint32_t ExternalMinorThreshold;        	  //!< External Disk Minor Threahold %
        uint32_t MaxConcurrentTransactions;       //!< Session Mgr Max Current Trans
        std::string SharedMemoryTmpFile;          //!< Session Mgr Shared Mem Temp file
        uint32_t NumVersionBufferFiles;       		//!< Version Buffer number of files
        uint32_t VersionBufferFileSize;       		//!< Version Buffer file size
        std::string OERYDBitmapFile;                  //!< OID Mgr Bitmap File name
        uint32_t FirstOID;       			        //!< OID Mgr First O
		std::string ParentOAMModule;				//!< Parent OAM Module Name
		std::string StandbyOAMModule;				//!< Standby Parent OAM Module Name
        uint32_t TransactionArchivePeriod;			//!< Tranaction Archive Period in minutes
    };
    typedef struct SystemConfig_s SystemConfig;

    /** @brief Host/IP Address Config Structure
     *
     */

    struct HostConfig_s
    {
        std::string HostName;         			//!< Host Name
        std::string IPAddr;                 	//!< IP address
        uint16_t NicID;                 		//!< NIC ID
    };
    typedef struct HostConfig_s HostConfig;

    /** @brief Host/IP Address Config List
     *
     */

	typedef std::vector<HostConfig> HostConfigList;

    /** @brief Device Network Config Structure
     *
     */

    struct DeviceNetworkConfig_s
    {
        std::string DeviceName;                 //!< Device Name
        std::string UserTempDeviceName;         //!< User Temp Device Name
        std::string DisableState;               //!< Disabled State
		HostConfigList hostConfigList;	        //!< IP Address and Hostname List
    };

    typedef struct DeviceNetworkConfig_s DeviceNetworkConfig;

    /** @brief Device Network Config List
     *
     */

	typedef std::vector<DeviceNetworkConfig> DeviceNetworkList;

    /** @brief Disk Monitor File System List
     *
     */

	typedef std::vector<std::string> DiskMonitorFileSystems;

    /** @brief DBRoot Config List
     *
     */

	typedef std::vector<uint16_t> DBRootConfigList;

    /** @brief Device DBRoot Config Structure
     *
     */

    struct DeviceDBRootConfig_s
    {
        uint16_t DeviceID;                 		//!< Device ID
		DBRootConfigList dbrootConfigList;	    //!< DBRoot List
    };

    typedef struct DeviceDBRootConfig_s DeviceDBRootConfig;

    /** @brief Device DBRoot Config List
     *
     */

	typedef std::vector<DeviceDBRootConfig> DeviceDBRootList;

    /** @brief Module Type Configuration Structure
     *
     *   Structure that is returned by the getSystemConfigFile API for the
     *   Module Type Configuration data stored in the System Configuration file
     */

    struct PmDBRootCount_s
    {
        uint16_t pmID;                 		//!< PM ID
		uint16_t count;	    				//!< DBRoot Count
    };

    struct ModuleTypeConfig_s
    {
        std::string ModuleType;                   //!< Module Type
        std::string ModuleDesc;                   //!< Module Description
        std::string RunType;                      //!< Run Type
        uint16_t ModuleCount;                     //!< Module Equipage Count
        uint16_t ModuleCPUCriticalThreshold;      //!< CPU Critical Threahold %
        uint16_t ModuleCPUMajorThreshold;         //!< CPU Major Threahold %
        uint16_t ModuleCPUMinorThreshold;         //!< CPU Minor Threahold %
        uint16_t ModuleCPUMinorClearThreshold;    //!< CPU Minor Clear Threahold %
        uint16_t ModuleMemCriticalThreshold;      //!< Mem Critical Threahold %
        uint16_t ModuleMemMajorThreshold;         //!< Mem Major Threahold %
        uint16_t ModuleMemMinorThreshold;         //!< Mem Minor Threahold %
        uint16_t ModuleDiskCriticalThreshold;     //!< Disk Critical Threahold %
        uint16_t ModuleDiskMajorThreshold;        //!< Disk Major Threahold %
        uint16_t ModuleDiskMinorThreshold;        //!< Disk Minor Threahold %
        uint16_t ModuleSwapCriticalThreshold;     //!< Swap Critical Threahold %
        uint16_t ModuleSwapMajorThreshold;        //!< Swap Major Threahold %
        uint16_t ModuleSwapMinorThreshold;        //!< Swap Minor Threahold %
		DeviceNetworkList ModuleNetworkList;	  //!< Module IP Address and Hostname List
        DiskMonitorFileSystems FileSystems; 	  //!< Module Disk File System list
		DeviceDBRootList ModuleDBRootList;		  //!< Module DBRoot 
    };
    typedef struct ModuleTypeConfig_s ModuleTypeConfig;


    /** @brief System Module Type Configuration Structure
     *
     *   Structure that is returned by the getSystemConfigFile API for the
     *   System Module Configuration data stored in the System Configuration file
     */

    struct SystemModuleTypeConfig_s
    {
        std::vector<ModuleTypeConfig> moduletypeconfig;   //!< Module Type Configuration Structure
    };
    typedef struct SystemModuleTypeConfig_s SystemModuleTypeConfig;

    /** @brief Module Name Configuration Structure
     *
     *   Structure that is returned by the getSystemConfigFile API for the
     *   Module Name Configuration data stored in the System Configuration file
     */

    struct ModuleConfig_s
    {
        std::string ModuleName;                   //!< Module Name
        std::string ModuleType;                   //!< Module Type
        std::string ModuleDesc;                   //!< Module Description
        std::string DisableState;                 //!< Disabled State
		HostConfigList hostConfigList;	          //!< IP Address and Hostname List
		DBRootConfigList dbrootConfigList;	      //!< DBRoot ID list
    };
    typedef struct ModuleConfig_s ModuleConfig;


    /** @brief External Device Name Configuration Structure
     *
     *   Structure that is returned by the getSystemConfigFile API for the
     *   External Device Name Configuration data stored in the System Configuration file
     */

    struct ExtDeviceConfig_s
    {
        std::string Name;                   		//!< Name
        std::string IPAddr;           	  			//!< IP address
        std::string DisableState;                 	//!< Disabled State
    };
    typedef struct ExtDeviceConfig_s ExtDeviceConfig;

    /** @brief System External Device Configuration Structure
     *
     *   Structure that is returned by the getSystemConfigFile API for the
     *   External Device Type Configuration data stored in the System Configuration file
     */

    struct SystemExtDeviceConfig_s
    {
        uint16_t Count;                     			//!< External Device Equipage Count
        std::vector<ExtDeviceConfig> extdeviceconfig;   //!< External Device IP Address and name List
    };
    typedef struct SystemExtDeviceConfig_s SystemExtDeviceConfig;

    /** @brief Module Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System Status data stored in the System Status file
     */

    struct ModuleStatus_s
    {
        std::string Module;                   //!< Module Name
        uint16_t ModuleOpState;                //!< Operational State
        std::string StateChangeDate;              //!< Last time/date state change
    };
    typedef struct ModuleStatus_s ModuleStatus;

    /** @brief System Module Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System Module Status data stored in the System Status file
     */

    struct SystemModuleStatus_s
    {
        std::vector<ModuleStatus> modulestatus;   //!< Module Status Structure
    };
    typedef struct SystemModuleStatus_s SystemModuleStatus;


    /** @brief Ext Device Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System Status data stored in the System Status file
     */

    struct ExtDeviceStatus_s
    {
        std::string Name;                   		//!< External Device Name
        uint16_t OpState;                			//!< Operational State
        std::string StateChangeDate;              	//!< Last time/date state change
    };
    typedef struct ExtDeviceStatus_s ExtDeviceStatus;

    /** @brief System Ext Device Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System System Ext Status data stored in the System Status file
     */

    struct SystemExtDeviceStatus_s
    {
        std::vector<ExtDeviceStatus> extdevicestatus;   //!< External Device Status Structure
    };
    typedef struct SystemExtDeviceStatus_s SystemExtDeviceStatus;


    /** @brief DBRoot Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System Status data stored in the System Status file
     */

    struct DbrootStatus_s
    {
        std::string Name;                   		//!< Dbroot Name
        uint16_t OpState;                			//!< Operational State
        std::string StateChangeDate;              	//!< Last time/date state change
    };
    typedef struct DbrootStatus_s DbrootStatus;

    /** @brief Dbroot Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System System Ext Status data stored in the System Status file
     */

    struct SystemDbrootStatus_s
    {
        std::vector<DbrootStatus> dbrootstatus;   //!< Dbroot Status Structure
    };
    typedef struct SystemDbrootStatus_s SystemDbrootStatus;

    /** @brief NIC Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System Status data stored in the System Status file
     */

    struct NICStatus_s
    {
        std::string HostName;                   //!< NIC Name
        uint16_t NICOpState;                	//!< Operational State
        std::string StateChangeDate;              //!< Last time/date state change
    };
    typedef struct NICStatus_s NICStatus;

    /** @brief System NIC Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System NIC Status data stored in the System Status file
     */

    struct SystemNICStatus_s
    {
        std::vector<NICStatus> nicstatus;   //!< NIC Status Structure
    };
    typedef struct SystemNICStatus_s SystemNICStatus;

    /** @brief System Status Structure
     *
     *   Structure that is returned by the getSystemStatus API for the
     *   System Status data stored in the System Status file
     */

    struct SystemStatus_s
    {
        uint16_t SystemOpState;                   		//!< System Operational State
        std::string StateChangeDate;              		//!< Last time/date state change
        SystemModuleStatus systemmodulestatus;    		//!< System Module status
        SystemExtDeviceStatus systemextdevicestatus;    //!< System Ext Device status
        SystemNICStatus systemnicstatus;    			//!< System NIC status
        SystemDbrootStatus systemdbrootstatus;    		//!< System DBroot status
    };
    typedef struct SystemStatus_s SystemStatus;

    /** @brief Process Configuration Structure
     *
     *   Structure that is returned by the getSystemProcessConfig API for the
     *   Process Configuration data stored in the Process Configuration file
     */

    struct ProcessConfig_s
    {
        std::string ProcessName;                  //!< Process Name
        std::string ModuleType;                   //!< Module Type that process is running on
        std::string ProcessLocation;              //!< Process launch location
        std::string ProcessArgs[MAX_ARGUMENTS];   //!< Process Arguments
        uint16_t    BootLaunch;                   //!< Boot Launch flag, 0 = init, 1 = boot, 2 = Mgr
        uint16_t    LaunchID;                     //!< Launch ID number
        std::string DepProcessName[MAX_DEPENDANCY]; //!< Dependent Processes
        std::string DepModuleName[MAX_DEPENDANCY];//!< Dependent Process Module Name
        std::string RunType;               	      //!< Process Run Type
        std::string LogFile;               	      //!< Process Log File Indicator
    };
    typedef struct ProcessConfig_s ProcessConfig;

    /** @brief System Process Configuration Structure
     *
     *   Structure that is returned by the getSystemProcessConfig API for the
     *   System Process Configuration data stored in the Process Configuration file
     */

    struct SystemProcessConfig_s
    {
        std::vector<ProcessConfig> processconfig; //!< Process Configuration Structure
    };
    typedef struct SystemProcessConfig_s SystemProcessConfig;

    /** @brief Process Status Structure
     *
     *   Structure that is returned by the getProcessStatus API for the
     *   Process Status data stored in the Process Status file
     */

    struct ProcessStatus_s
    {
        std::string ProcessName;                  //!< Process Name
        std::string Module;                   //!< Module Name that process is running on
        pid_t    	ProcessID;                    //!< Process ID number
        std::string StateChangeDate;              //!< Last time/date state change
        uint16_t 	ProcessOpState;               //!< Process Operational State
    };	
    typedef struct ProcessStatus_s ProcessStatus;


    /** @brief System Process Status Structure
     *
     *   Structure that is returned by the getProcessStatus API for the
     *   System Process Status data stored in the Process Status file
     */

    struct SystemProcessStatus_s
    {
        std::vector<ProcessStatus> processstatus; //!< Process Status Structure
    };
    typedef struct SystemProcessStatus_s SystemProcessStatus;

    /** @brief Alarm Configuration Structure
     *
     *   Structure that is returned by the getAlarmConfig API for the
     *   Alarm Configuration data stored in the Alarm Configuration file
     */

    struct AlarmConfig_s
    {
        uint16_t AlarmID;                         //!< Alarm ID
        std::string BriefDesc;                    //!< Brief Description
        std::string DetailedDesc;                 //!< Detailed Description
        uint16_t Severity;                        //!< Severity - 1=Critical, 2=Major, 3=Minor, 4=Warning, 5=Informational
        uint16_t Threshold;                       //!< Stop reporting threshold
        uint16_t Occurrences;                     //!< Alarm Occurrences within 30 min window
        uint32_t LastIssueTime;                   //!< last time alarms was issued
    };
    typedef struct AlarmConfig_s AlarmConfig;

    /** @brief Local Module OAM Configuration StructureLOG_
     *
     *   Structure that is returned by the getModuleInfo API for the
     *   Local Module OAM Configuration data stored in the Local Module
     *   Configuration file
	 *   Returns: Local Module Name, Local Module Type, Local Module ID, 
	 *					OAM Parent Module Name, OAM Parent Flag,
	 *					Server Type Install ID, OAM Standby Parent Module Name,
	 *					OAM Standby Parent Flag,
     */

    typedef boost::tuple<std::string, std::string, uint16_t, std::string, bool, uint16_t, std::string, bool > oamModuleInfo_t;

    /** @brief My Process OAM Status Structure
     *
     *   Structure that is returned by the getMyProcessStatus API for the
     *   Local Process OAM Status data stored in the Process Status file
     *	 Returns: Process ID, Process Name, and Process State
     */

    typedef boost::tuple<uint16_t, std::string, uint16_t> myProcessStatus_t;

    /** @brief Store Device ID Structure
     *
     *   Structure that is returned by the getMyProcessStatus API for the
     *   Local Process OAM Status data stored in the Process Status file
     *	 Returns: Process ID, Process Name, and Process State
     */

    typedef boost::tuple<std::string, std::string> storageID_t;

    /** @brief User Configuration Structure
     *
     *   Structure that is returned by the getHardwareUserConfig API for the
     *   User Configuration data
     */

    struct UserConfig_s
    {
        std::string UserName;                     //!< User Name
        AUTH_LEVEL UserAuthLevel;                 //!< User Authorization level
        bool UserActiveFlag;                      //!< User Actively logged in
    };
    typedef struct UserConfig_s UserConfig;

    /** @brief System User Configuration Structure
     *
     *   Structure that is returned by the getHardwareUserConfig API for the
     *   System User Configuration data
     */

    struct SystemUserConfig_s
    {
        std::vector<UserConfig> userconfig;       //!< User Configuration Structure
    };
    typedef struct SystemUserConfig_s SystemUserConfig;



    /** @brief Process Cpu User Structure
     *
     */

    struct ProcessCpuUser_s
    {
        std::string ProcessName;              	//!< Process Name
        uint16_t 	CpuUsage;               	//!< Process Cpu Usage %
    };	
    typedef struct ProcessCpuUser_s ProcessCpuUser;

    /** @brief TOP Process Cpu User Structure
     *
     *   Structure that is returned by the getTopProcessCpuUsers API
     */

    struct TopProcessCpuUsers_s
    {
        std::string ModuleName;                    		//!< Module Name
        uint16_t    numberTopUsers;                    	//!< Number of TOP Users
        std::vector<ProcessCpuUser> processcpuuser;   	//!< TOP Users
    };	
    typedef struct TopProcessCpuUsers_s TopProcessCpuUsers;

    /** @brief System TOP Process Cpu User Structure
     *
     *   Structure that is returned by the getTopProcessCpuUsers API
     */

    struct SystemTopProcessCpuUsers_s
    {
        std::vector<TopProcessCpuUsers> topprocesscpuusers; //!< TOP Process Cpu User Structure
    };
    typedef struct SystemTopProcessCpuUsers_s SystemTopProcessCpuUsers;

    /** @brief Module Cpu Structure
     *
     */

    struct ModuleCpu_s
    {
        std::string ModuleName;              	//!< Module Name
        uint16_t 	CpuUsage;               	//!< Module Cpu Usage %
    };	
    typedef struct ModuleCpu_s ModuleCpu;

    /** @brief System Module Cpu Structure
     *
     *   Structure that is returned by the getTopProcessCpuUsers API
     */

    struct SystemCpu_s
    {
        std::vector<ModuleCpu> modulecpu; //!< Module Cpu
    };
    typedef struct SystemCpu_s SystemCpu;


    /** @brief Process Memory User Structure
     *
     */

    struct ProcessMemoryUser_s
    {
        std::string ProcessName;              	//!< Process Name
        uint32_t 	MemoryUsed;               	//!< Process Memory Used
        uint16_t 	MemoryUsage;               	//!< Process Memory Usage %
    };	
    typedef struct ProcessMemoryUser_s ProcessMemoryUser;

    /** @brief TOP Process Memory User Structure
     *
     *   Structure that is returned by the getTopProcessMemoryUsers API
     */

    struct TopProcessMemoryUsers_s
    {
        std::string ModuleName;                    		//!< Module Name
        uint16_t    numberTopUsers;                    	//!< Number of TOP Users
        std::vector<ProcessMemoryUser> processmemoryuser; //!< TOP Users
    };	
    typedef struct TopProcessMemoryUsers_s TopProcessMemoryUsers;

    /** @brief System TOP Process Memory User Structure
     *
     *   Structure that is returned by the getTopProcessMemoryUsers API
     */

    struct SystemTopProcessMemoryUsers_s
    {
        std::vector<TopProcessMemoryUsers> topprocessmemoryusers; //!< TOP Process Memory User Structure
    };
    typedef struct SystemTopProcessMemoryUsers_s SystemTopProcessMemoryUsers;

    /** @brief Module Memory Structure
     *
     */

    struct ModuleMemory_s
    {
        std::string ModuleName;              	//!< Module Name
		uint32_t MemoryTotal;              		//!< Memory Total
		uint32_t MemoryUsed;              		//!< Memory Used
		uint32_t cache;              			//!< Cache Used
		uint16_t MemoryUsage;              		//!< Memory Usage Percent
		uint32_t SwapTotal;              		//!< Swap Total
		uint32_t SwapUsed;              		//!< Swap Used
		uint16_t SwapUsage;              		//!< Swap Usage Percent
    };	
    typedef struct ModuleMemory_s ModuleMemory;

    /** @brief System Module Cpu Structure
     *
     *   Structure that is returned by the getTopProcessCpuUsers API
     */

    struct SystemMemory_s
    {
        std::vector<ModuleMemory> modulememory; //!< Module Memory
    };
    typedef struct SystemMemory_s SystemMemory;

    /** @brief Disk Usage Structure
     *
     */

    struct DiskUsage_s
    {
        std::string DeviceName;              	//!< Device Name
		uint64_t TotalBlocks;					//!< Total Blocks
		uint64_t UsedBlocks;					//!< Used Blocks
        uint16_t DiskUsage;               		//!< Disk Usage %
    };	
    typedef struct DiskUsage_s DiskUsage;

    /** @brief Module Disk Usage Structure
     *
     *   Structure that is returned by the getTopProcessMemoryUsers API
     */

    struct ModuleDisk_s
    {
        std::string ModuleName;               	//!< Module Name
        std::vector<DiskUsage> diskusage;   	//!< Disk Usage
    };	
    typedef struct ModuleDisk_s ModuleDisk;

    /** @brief System Disk Usage Structure
     *
     *   Structure that is returned by the getTopProcessMemoryUsers API
     */

    struct SystemDisk_s
    {
        std::vector<ModuleDisk> moduledisk; 	//!< Module Disk Usage
    };
    typedef struct SystemDisk_s SystemDisk;

    /** @brief Active Sql Statement Structure
     *
     */

    struct ActiveSqlStatement
    {
        std::string sqlstatement;
        unsigned    starttime;
        uint64_t    sessionid;
    };	
    typedef std::vector<ActiveSqlStatement> ActiveSqlStatements;


    // username / password for smbclient use
	const std::string USERNAME = "oamuser";
	const std::string PASSWORD = "erydb1"; 

    /** @brief System Storage Configuration Structure
     *
     *   Structure that is returned by the getStorageConfig API
	 *   Returns: Storage Type, System DBRoot count, PM dbroot info, 
     */

    typedef boost::tuple<std::string, uint16_t, DeviceDBRootList, std::string > systemStorageInfo_t;

	typedef std::vector<std::string> dbrootList;

}	// end of namespace
 
#undef EXPORT
using namespace oam;
#endif
// vim:ts=4 sw=4:

