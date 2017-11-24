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
#ifndef LIBOAMCPP_H
#define LIBOAMCPP_H

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

#include "bytestream.h"
#include "configcpp.h"
#include "boost/tuple/tuple.hpp"
#include "alarmmanager.h"
#include "dbrm.h"
#include "liboam.h"

#include "messagequeue.h"

#if defined(_MSC_VER) && defined(xxxLIBOAM_DLLEXPORT)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

namespace oam
{
    /** @brief OAM API I/F class
     *
     *  Operations, Administration, and Maintenance C++ APIs. These APIS are utilized
     *  to Configure the Hardware and the Software on the erydb Database Appliance.
     *  They are also used to retrieve the configuration, hardware and process status,
     *  alarms, logs, and performance data.
     *
     */

    class Oam
    {
		long eryDBSystemRunFlagcheckTime;
		bool eryDBSystemRuningFlag;


        public:
            /** @brief OAM C++ API Class constructor
             */
            EXPORT Oam();

            /** @brief OAM C++ API Class destructor
             */
            EXPORT virtual ~Oam();
            EXPORT bool checkSystemRunFlag();
            /** @brief get System Software information
             *
             * get System Software information from the System software RPM.
             * @param systemconfig Returned System Software Structure
             */
            EXPORT void getSystemSoftware(SystemSoftware& systemsoftware);

            /** @brief get System Configuration information
             *
             * get System Configuration information from the system config file.
             * @param systemconfig Returned System Configuration Structure
             */
            EXPORT void getSystemConfig(SystemConfig& systemconfig);

            /** @brief get System Module Configuration information
             *
             * get System Module Configuration information value from the system config file.
             * @param systemmoduletypeconfig Returned System Module Configuration Structure
             */
            EXPORT void getSystemConfig(SystemModuleTypeConfig& systemmoduletypeconfig);

            /** @brief get System Module Type Configuration information for a Module
             *
             * get System Module Type Configuration information for a Module from the system config file.
             * @param moduletype the Module Type to get information
             * @param moduletypeconfig Returned System Module Configuration Structure
             */
            EXPORT void getSystemConfig(const std::string&moduletype, ModuleTypeConfig& moduletypeconfig);

            /** @brief get System Module Name Configuration information for a Module
             *
             * get System Module Name Configuration information for a Module from the system config file.
             * @param moduleName the Module Name to get information
             * @param moduleconfig Returned System Module Configuration Structure
             */
            EXPORT void getSystemConfig(const std::string&moduleName, ModuleConfig& moduleconfig);

            /** @brief get System Module Configuration information for local Module
             *
             * get System Module Name Configuration information for local Module from the system config file.
             * @param Moduleconfig Returned System Configuration Structure
             */
            EXPORT void getSystemConfig(ModuleConfig& moduleconfig);

            /** @brief get System Module Type Configuration information for local Module Type
             *
             * get System Module Name Configuration information for local Module from the system config file.
             * @param moduletypeconfig Returned System Configuration Structure
             */
            EXPORT void getSystemConfig(ModuleTypeConfig& moduletypeconfig);

            /** @brief get System Ext Device Name Configuration information
             *
             * get System Ext Device Name Configuration information for a System Ext from the system config file.
             * @param name the Ext Device Name to get information
             * @param extdeviceConfig Returned System Ext Device Configuration Structure
             */
            EXPORT void getSystemConfig(const std::string&name, ExtDeviceConfig& extdeviceConfig);

            /** @brief get System Ext Device Configuration information
             *
             * get System Ext Device Name Configuration information for local System Ext from the system config file.
             * @param extdeviceConfig Returned System Configuration Structure
             */
            EXPORT void getSystemConfig(SystemExtDeviceConfig& systemextdeviceConfig);

            /** @brief set Ext Device Configuration information
             *
             * Set Ext Device Configuration information
             * @param deviceName the Device Name to get information
             * @param extdeviceConfig Ext Device Configuration Structure
             */
    		EXPORT void setSystemConfig(const std::string deviceName, ExtDeviceConfig extdeviceConfig);

           /** @brief get System Configuration String Parameter
             *
             * get System Configuration String Parameter from the system config file.
             * @param name the Parameter Name to get value
             * @param value Returned Parameter Value
             */
            EXPORT void getSystemConfig(const std::string&name, std::string& value);

            /** @brief get System Configuration Integer Parameter
             *
             * get System Configuration Integer Parameter from the system config file.
             * @param name the Parameter Name to get value
             * @param value Returned Parameter Value
             */
            EXPORT void getSystemConfig(const std::string&name, int& value);

            /** @brief get Module Name for IP Address
             *
             * get Module Name for given IP address from the system config file.
             * @param IpAddress the Patamater IP Address
             * @param moduleName Returned Parameter Value
             */
            EXPORT void getModuleNameByIPAddr(const std::string IpAddress, std::string& moduleName);

            /** @brief set System Configuration String Parameter
             *
             * set System Configuration String Parameter from the system config file.
             * @param name the Parameter Name to set value
             * @param value the Parameter Value to set
             */
            EXPORT void setSystemConfig(const std::string name, const std::string value);

            /** @brief set System Configuration Integer Parameter
             *
             * set System Configuration Integer Parameter from the system config file.
             * @param name the Parameter Name to set value
             * @param value the Parameter Value to set
             */
            EXPORT void setSystemConfig(const std::string name, const int value);

            /** @brief set System Module Type Configuration information for a Module
             *
             * set System Module Type Configuration information for a Module from the system config file.
             * @param moduletype the Module Type to get information
             * @param moduletypeconfig System Module Configuration Structure
             */
            EXPORT void setSystemConfig(const std::string moduletype, ModuleTypeConfig moduletypeconfig);


            /** @brief set System Module Name Configuration information for a Module
             *
             * Set System Module Name Configuration information for a Module from the system config file.
             * @param moduleName the Module Name to get information
             * @param moduleconfig System Module Configuration Structure
             */
    		EXPORT void setSystemConfig(const std::string module, ModuleConfig moduleconfig);


            /** @brief add Module
             *
             * Add module to the system config file.
             * @param DeviceNetworkConfig the Modules added
             * @param password Host Root Password
            */
            EXPORT void addModule(DeviceNetworkList devicenetworklist, const std::string password, const std::string mysqlpw);

            /** @brief remove Module
             *
             * Remove module from the system config file.
             * @param DeviceNetworkConfig the Modules to be removed
             */
            EXPORT void removeModule(DeviceNetworkList devicenetworklist);

            /** @brief reconfigure Module
             *
             * Add module to the system config file.
             * @param DeviceNetworkConfig the Module Name to be reconfigured
             */
            EXPORT void reconfigureModule(DeviceNetworkList devicenetworklist);

            /** @brief get System Status information
             *
             * get System Status information from the system status file.
             * @param systemstatus Returned System Status Structure
             */
            EXPORT void getSystemStatus(SystemStatus& systemstatus, bool systemStatusOnly = true);

            /** @brief set System Status information
             *
             * set System Status information in the system status file.
             * @param state System Operational State
             */
            EXPORT void setSystemStatus(const int state);

            /** @brief get Module Status information
             *
             * get Module Status information from the system status file.
             * @param name Module Name
             * @param state Returned Operational State
             */
            EXPORT void getModuleStatus(const std::string name, int& state, bool& degraded);

            /** @brief set Module Status information
             *
             * set Module Status information in the system status file.
             * @param name Module Name
             * @param state Module Operational State
             */
            EXPORT void setModuleStatus(const std::string name, const int state);

            /** @brief get Ext Device Status information
             *
             * get Ext Device Status information from the system status file.
             * @param name Ext Device Name
             * @param state Returned Operational State
             */
            EXPORT void getExtDeviceStatus(const std::string name, int& state);

            /** @brief set Ext Device Status information
             *
             * set Ext Device Status information in the system status file.
             * @param name Ext Device Name
             * @param state System Ext Operational State
             */
            EXPORT void setExtDeviceStatus(const std::string name, const int state);

            /** @brief get Dbroot Status information
             *
             * get DBroot Status information in the system status file.
             * @param name DBroot Name
             * @param state System  Operational State
             */
            EXPORT void getDbrootStatus(const std::string name, int& state);

            /** @brief set Dbroot Status information
             *
             * set DBroot Status information in the system status file.
             * @param name DBroot Name
             * @param state System  Operational State
             */
            EXPORT void setDbrootStatus(const std::string name, const int state);

            /** @brief get NIC Status information
             *
             * get NIC Status information.
             * @param name NIC HostName
             * @param state Returned Operational State
             */
            EXPORT void getNICStatus(const std::string name, int& state);

            /** @brief set NIC Status information
             *
             * set NIC Status information.
             * @param name NIC HostName
             * @param state NIC Operational State
             */
            EXPORT void setNICStatus(const std::string name, const int state);

            /** @brief get System Process Configuration information
             *
             * get System Configuration Process information from the Process config file.
             * @param systemprocessconfig Returned System Process Configuration Structure
             */
            EXPORT void getProcessConfig(SystemProcessConfig& systemprocessconfig);

            /** @brief get Process Configuration information
             *
             * get System Process information from the Process config file.
             * @param process the Process Name to get value
             * @param module the Module Name for the Process to get value
             * @param processconfig Returned Process Configuration Structure
             */
            EXPORT void getProcessConfig(const std::string process, const std::string module, ProcessConfig& processconfig);

            /** @brief get Process Configuration String Parameter
             *
             * get Process Configuration String Parameter from the Process config file.
             * @param process the Process Name to get value
             * @param module the Module Name for the Process to get value
             * @param name the Parameter Name to get value
             * @param value the Parameter Value to get
             */

            EXPORT void getProcessConfig(const std::string process, const std::string module, const std::string name, std::string& value);

            /** @brief get Process Configuration Integer Parameter
             *
             * get Process Configuration Integer Parameter from the Process config file.
             * @param process the Process Name to get value
             * @param module the Module Name for the Process to get value
             * @param name the Parameter Name to get value
             * @param value the Parameter Value to get
             */
            EXPORT void getProcessConfig(const std::string process, const std::string module, const std::string name, int& value);

            /** @brief set Process Configuration String Parameter
             *
             * set Process Configuration String Parameter from the Process config file.
             * @param process the Process Name to set value
             * @param module the Module Name for the Process to set value
             * @param name the Parameter Name to set value
             * @param value the Parameter Value to set
             */

            EXPORT void setProcessConfig(const std::string process, const std::string module, const std::string name, const std::string value);

            /** @brief set Process Configuration Integer Parameter
             *
             * set Process Configuration Integer Parameter from the Process config file.
             * @param process the Process Name to set value
             * @param module the Module Name for the Process to set value
             * @param name the Parameter Name to set value
             * @param value the Parameter Value to set
             */
            EXPORT void setProcessConfig(const std::string process, const std::string module, const std::string name, const int value);

            /** @brief get System Process Status information
             *
             * get System Process Status information from the Process status  file.
             * @param systemprocessconfig Returned System Process Status Structure
             */
            EXPORT void getProcessStatus(SystemProcessStatus& systemprocessstatus, std::string port = "ProcStatusControl");

            /** @brief get Process Status information
             *
             * get Process information from the Process Status file.
             * @param process the Process Name to get value
             * @param module the Module Name for the Process to get value
             * @param processconfig Returned Process Status Structure
             */
            EXPORT void getProcessStatus(const std::string process, const std::string module, ProcessStatus& processstatus);

            /** @brief set Process Status
             *
             * set Process Status
             * @param process the Process Name to set value
             * @param module the Module Name for the Process to set value
             * @param state the Operational state
             * @param PID the Process ID
             */

            EXPORT void setProcessStatus(const std::string process, const std::string module, const int state, pid_t PID);

            /** @brief Process Init Complete
             *
             * Process Init Complete
             * 
             */

            EXPORT void processInitComplete(std::string processName, int STATE = oam::ACTIVE);

            /** @brief Process Init Failure
             *
             * Process Init Failure
             * 
             */

            EXPORT void processInitFailure();

            /** @brief get Local Process Status Data
             *
             * get Local PID, Name, and Status from Process Status file
             * @return myProcessStatus_t structure, which contains the local process OAM
             *         Status Data
             */
            EXPORT myProcessStatus_t getMyProcessStatus(pid_t processID = 0);

            /** @brief get Local Module Configuration Data
             *
             * get Local Module Name, OAM Parent Flag, and Realtime Linux OS Flag from
             * local config file.
             * @return oamModuleInfo_t structure, which contains the local Module OAM
             *         Configuration Data
             */
            EXPORT oamModuleInfo_t getModuleInfo();

            /** @brief get Alarm Configuration information
             *
             * get Alarm Configuration information from the alarm config file.
             * @param alarmid the Alarm ID for the parameter value
             * @param alarmconfig Returned Alarm Configuration Structure
             */
            EXPORT void getAlarmConfig(const int alarmid, AlarmConfig& alarmconfig);

            /** @brief get Alarm Configuration String Parameter
             *
             * get Alarm Configuration String Parameter from the Alarm config file.
             * @param alarmid the Alarm ID to get Alarm Configuration information
             * @param name the Parameter Name for the parameter value
             * @param value returned Parameter Value
             */
            EXPORT void getAlarmConfig(const int alarmid, const std::string name, std::string& value);

            /** @brief get Alarm Configuration Integer Parameter
             *
             * get Alarm Configuration Integer Parameter from the Alarm config file.
             * @param alarmid the Alarm ID to get the parameter value
             * @param name the Parameter Name for the parameter value
             * @param value returned Parameter Value
             */
            EXPORT void getAlarmConfig(const int alarmid, const std::string name, int& value);

            /** @brief set Alarm Configuration String Parameter
             *
             * set Alarm Configuration String Parameter from the Alarm config file.
             * @param alarmid the Alarm ID to set the parameter value
             * @param name the Parameter Name to set
             * @param value the Parameter Value to set
             */
            EXPORT void setAlarmConfig(const int alarmid, const std::string name, const std::string value);

            /** @brief set Alarm Configuration Integer Parameter
             *
             * set Alarm Configuration Integer Parameter from the Alarm config file.
             * @param alarmid the Alarm ID to set the parameter value
             * @param name the Parameter Name to set
             * @param value the Parameter Value to set
             */
            EXPORT void setAlarmConfig(const int alarmid, const std::string name, const int value);

            /** @brief OAM Hardware Management Login
             *
             * Login into the system to utilizes the OAM APIs from a user application
             * @param username the Login User Name
             * @param password the Login Password
             */
            EXPORT void login(const std::string username, const std::string password);

            /** @brief OAM Hardware Management Self Logout
             *
             * Logout from OAM Hardware Management system
             */
            EXPORT void logout();

            /** @brief OAM Hardware Management Logout
             *
             * Logout another user from OAM Hardware Management system
             * @param username the Login User Name
             * @param password the Login Password
             */
            EXPORT void logout(const std::string username, const std::string password);

            /** @brief Add OAM Hardware Management User
             *
             * Add a new user to the OAM Hardware Management system
             * @param username the new User Name
             * @param password the new User Password
             * @param authlevel the Authorization Level for the new user
             */
            EXPORT void addHardwareUser(const std::string username, const std::string password, AUTH_LEVEL authlevel);

            /** @brief Change OAM Hardware Management User Password
             *
             * Change a current OAM Hardware Management User's password
             * @param username the User Name
             * @param oldpassword the old User Password
             * @param newpassword the new User Password
             */
            EXPORT void changeHardwareUserPassword(const std::string username, const std::string oldpassword, const std::string newpassword);

            /** @brief Delete OAM Hardware Management User
             *
             * Delete a current OAM Hardware Management User
             * @param username the User Name
             */
            EXPORT void deleteHardwareUser(const std::string username);

            /** @brief Get all OAM Hardware Management User Configuration
             *
             * Get OAM Hardware Management User Configuration for a single user
             * @param systemuserconfig Returned System User Configuration Structure
             */
            EXPORT void getHardwareUserConfig(SystemUserConfig& systemuserconfig);

            /** @brief Get OAM Hardware Management User Configuration
             *
             * Get all OAM Hardware Management User Configuration
             * @param username the User Name
             * @param userconfig Returned User Configuration Structure
             */
            EXPORT void getHardwareUserConfig(const std::string username, UserConfig& userconfig);

            /** @brief Stop Module
             *
             * Stop's a Module within the erydb Database Appliance
             * @param name the Module Name to stop
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
//            EXPORT void stopModule(const std::string name, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Shutdown Module
             *
             * Shutdown's a Module within the erydb Database Appliance
             * @param name the Module Name to stop
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
//            EXPORT void shutdownModule(const std::string name, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Start Module
             *
             * Start's a stopped Module within the erydb Database Appliance
             * @param name the Module Name to stop
             * @param ackflag Acknowledgment flag
             */
//            EXPORT void startModule(const std::string name, ACK_FLAG ackflag);

            /** @brief Restart Module
             *
             * Restart's a Module within the erydb Database Appliance
             * @param name the Module Name to restart
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
//            EXPORT void restartModule(const std::string name, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Disable Module
             *
             * Disable a Module within the erydb Database Appliance
             * @param name the Module Name to disable
             */
//            EXPORT void disableModule(const std::string name);

            /** @brief Enable Module
             *
             * Enable a Module within the erydb Database Appliance
             * @param name the Module Name to enable
             */
//            EXPORT void enableModule(const std::string name);

            /** @brief Stop Module
             *
             * Stop's a Module within the erydb Database Appliance
             * @param DeviceNetworkConfig the Modules to be stopped
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void stopModule(DeviceNetworkList devicenetworklist, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Shutdown Module
             *
             * Shutdown's a Module within the erydb Database Appliance
             * @param DeviceNetworkConfig the Modules to be shutdown
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void shutdownModule(DeviceNetworkList devicenetworklist, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Start Module
             *
             * Start's a stopped Module within the erydb Database Appliance
             * @param DeviceNetworkConfig the Modules to be started
             * @param ackflag Acknowledgment flag
             */
            EXPORT void startModule(DeviceNetworkList devicenetworklist, ACK_FLAG ackflag);

            /** @brief Restart Module
             *
             * Restart's a Module within the erydb Database Appliance
             * @param DeviceNetworkConfig the Modules to be restarted
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void restartModule(DeviceNetworkList devicenetworklist, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Disable Module
             *
             * Disable a Module within the erydb Database Appliance
             * @param DeviceNetworkConfig the Modules to be disabled
             */
            EXPORT void disableModule(DeviceNetworkList devicenetworklist);

            /** @brief Enable Module
             *
             * Enable a Module within the erydb Database Appliance
             * @param DeviceNetworkConfig the Modules to be enabled
             */
            EXPORT void enableModule(DeviceNetworkList devicenetworklist);

            /** @brief Stop System
             *
             * Stop's the erydb Database Appliance System
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void stopSystem(GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Shutdown System
             *
             * Shutdown's the erydb Database Appliance System
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void shutdownSystem(GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Suspend Database Writes
             *
             * Suspends writing to the database. This should be done before backup 
             * activities occur. 
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void SuspendWrites(GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Start System
             *
             * Start's the stopped erydb Database Appliance System
             * @param ackflag Acknowledgment flag
             */
            EXPORT void startSystem(ACK_FLAG ackflag);

            /** @brief Restart System
             *
             * Restart's the active/stopped erydb Database Appliance System
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT int restartSystem(GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Display a list of locked tables
             */
            void DisplayLockedTables(std::vector<BRM::TableLockInfo>& tableLocks, BRM::DBRM* pDBRM = NULL);

            /** @brief Get Active Alarms
             *
             * Get's the Active Alarm list for the erydb Database Appliance
             * @param activealarm Returned Active Alarm list Structure
             */
            EXPORT void getActiveAlarms(alarmmanager::AlarmList& activealarm);

            /** @brief Get Historical Alarms
             *
             * Get's the Alarm list for the erydb Database Appliance
			 * @param date date of alarms, "today" or date in YYYYMMDD
             * @param activealarm Returned Alarm list Structure
             */
            EXPORT void getAlarms(std::string date, alarmmanager::AlarmList& alarm);

            /** @brief check Active Alarm
             *
             * Check if alarm is in Active Alarm file
             * @param alarmid the Alarm ID
             * @param moduleName the Module Name
             * @param deviceName the Alarm device Name
             */
            EXPORT bool checkActiveAlarm(const int alarmid, const std::string moduleName, const std::string deviceName);

            /** @brief Read Active Alarms in HDFS
             *
             * Read the Active Alarm list from the HDFS image of activeAlarms
             * @param activealarm Returned Active Alarm list Structure
             */
            EXPORT int readHdfsActiveAlarms(alarmmanager::AlarmList& activealarm);

            /** @brief update Log
             *
             * Enable/Disable Logging with the system or on a specific Module at
             * a specific level
             * @param action enabled/disable
             * @param deviceid the device which logging is being enabled/disable
             * @param loglevel the level of logging being enabled/disable
             */
            EXPORT void updateLog(const std::string action, const std::string deviceid, const std::string loglevel);

            /** @brief get Log data file location for today
             *
             * Get Log file location for specific Module at a specific level
             * @param moduleName the Module where the log file is located
             * @param loglevel the level of logging
             * @param filelocation Returned: the location path of the log file
             */
            EXPORT void getLogFile(const std::string moduleName, const std::string loglevel, std::string& filelocation);

            /** @brief get Log data file location
             *
             * Get Log file location for specific Module at a specific level
             * @param moduleName the Module where the log file is located
             * @param loglevel the level of logging
             * @param date date of log file, either "today" or 
             * @param filelocation Returned: the location path of the log file
             */
            EXPORT void getLogFile(const std::string moduleName, const std::string loglevel, const std::string date, std::string& filelocation);

            /** @brief get Log configuration data
             *
             * Get Log Config data, which is the File IDs in the Module syslog.conf file
             * @param moduleName the Module where the log file is located
             * @param fileIDs Returned: list of Log File IDs
             */
            EXPORT void getLogConfig(SystemLogConfigData& configData );

            /** @brief get current time in string format
             *
             * get current time in string format
             */
            EXPORT std::string getCurrentTime();

            /** @brief get free diskspace in bytes
             *
             */
            EXPORT double getFreeSpace(std::string path);

            // Integer to ASCII convertor

            EXPORT std::string itoa(const int);

            /** @brief Stop Process
             *
             * Stop's a process on a Module within the erydb Database Appliance
             * @param moduleName the Module Name
             * @param processName the Process Name to stopped
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void stopProcess(const std::string moduleName, const std::string processName, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Start Process
             *
             * Start's a process on a Module within the erydb Database Appliance
             * @param moduleName the Module Name
             * @param processName the Process Name to started
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void startProcess(const std::string moduleName, const std::string processName, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

            /** @brief Restart Process
             *
             * Restart's a process on a Module within the erydb Database Appliance
             * @param moduleName the Module Name
             * @param processName the Process Name to restarted
             * @param gracefulflag Graceful/Forceful flag
             * @param ackflag Acknowledgment flag
             */
            EXPORT void restartProcess(const std::string moduleName, const std::string processName, GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag);

              /** @brief Stop Process Type
             *
             * Stop's processes within the erydb Database Appliance
             */
            EXPORT void stopProcessType(std::string type);

            /** @brief Start Process Type
             *
             * Start's processes within the erydb Database Appliance
             */
            EXPORT void startProcessType(std::string type);

            /** @brief Restart Process Type
             *
             * Restart's process within the erydb Database Appliance
             */
            EXPORT void restartProcessType(std::string type);

            /** @brief Reinit Process Type
             *
             * Reinit's process within the erydb Database Appliance
             */
            EXPORT void reinitProcessType(std::string type);

            /** @brief Get Local DBRM ID for Module
			 *
             * @param moduleName the Module Name
             */
            EXPORT int getLocalDBRMID(const std::string moduleName);

            /** @brief build empty set of System Tables
             */
			EXPORT void buildSystemTables();

            /** @brief local exception control function
             * @param function Function throwing the exception
             * @param returnStatus 
             * @param msg A message to be included 
             */
            EXPORT void exceptionControl(std::string function, int returnStatus, const char* extraMsg = NULL);

            /** @brief get IP Address from Hostname
             */
			EXPORT std::string getIPAddress(std::string hostName);

            /** @brief get System TOP Process CPU Users 
             *
             * get System TOP Process CPU Users 
             * @param topNumber Number of TOP processes to retrieve
             * @param systemtopprocesscpuusers Returned System Top Process CPU Users Structure
             */
            EXPORT void getTopProcessCpuUsers(int topNumber, SystemTopProcessCpuUsers& systemtopprocesscpuusers);

            /** @brief get Module TOP Process CPU Users 
             *
             * get Module TOP Process CPU Users 
             * @param topNumber Number of TOP processes to retrieve
             * @param topprocesscpuusers Returned Top Process CPU Users Structure
             */
            EXPORT void getTopProcessCpuUsers(const std::string module, int topNumber, TopProcessCpuUsers& topprocesscpuusers);

            /** @brief get System CPU Usage
             *
             * get System CPU Usage
             * @param systemcpu Returned System CPU Usage Structure
             */
            EXPORT void getSystemCpuUsage(SystemCpu& systemcpu);

            /** @brief get Module CPU Usage
             *
             * get Module CPU Usage
             * @param module Module Name
             * @param modulecpu Returned Top Process CPU Users Structure
             */
            EXPORT void getModuleCpuUsage(const std::string module, ModuleCpu& modulecpu);

            /** @brief get System TOP Process Memory Users 
             *
             * get System TOP Process Memory Users 
             * @param topNumber Number of Memory processes to retrieve
             * @param systemtopprocessmemoryusers Returned System Top Process Memory Users Structure
             */
            EXPORT void getTopProcessMemoryUsers(int topNumber, SystemTopProcessMemoryUsers& systemtopprocessmemoryusers);

            /** @brief get Module TOP Process Memory Users 
             *
             * get Module TOP Process Memory Users 
             * @param module Module Name
             * @param topNumber Number of TOP processes to retrieve
             * @param topprocessmemoryusers Returned Top Process Memory Users Structure
             */
            EXPORT void getTopProcessMemoryUsers(const std::string module, int topNumber, TopProcessMemoryUsers& topprocessmemoryusers);

            /** @brief get System Memory Usage
             *
             * get System Memory Usage
             * @param systemmemory Returned System memory Usage Structure
             */
            EXPORT void getSystemMemoryUsage(SystemMemory& systemmemory);

            /** @brief get Module Memory Usage
             *
             * get Module Memory Usage
             * @param module Module Name
             * @param modulememory Returned Module Memory Usage Structure
             */
            EXPORT void getModuleMemoryUsage(const std::string module, ModuleMemory& modulememory);

            /** @brief get System Disk Usage
             *
             * get System Disk Usage
             * @param systemdisk Returned System Disk Usage Structure
             */
            EXPORT void getSystemDiskUsage(SystemDisk& systemdisk);

            /** @brief get Module Disk Usage
             *
             * get Module Disk Usage
             * @param module Module Name
             * @param moduledisk Returned Module Disk Usage Structure
             */
            EXPORT void getModuleDiskUsage(const std::string module, ModuleDisk& moduledisk);

            /** @brief get Active SQL Statements
             *
             * get Active SQL Statements
             * @param activesqlstatements Returned Active Sql Statement Structure
             */
            EXPORT void getActiveSQLStatements(ActiveSqlStatements& activesqlstatements);

            /** @brief Valid IP Address
             *
             * Validate IP Address format
             */
			EXPORT bool isValidIP(const std::string ipAddress);

            /** @brief Increment IP Address
             *
             * Increment IP Address
             */
		EXPORT std::string incrementIPAddress(const std::string ipAddress);

		/**
		*@brief Check for a phrase in a log file and return status
		*/
		EXPORT bool checkLogStatus(std::string filename, std::string phase);

		/**
		*@brief  Fix RSA key
		*/
		EXPORT void fixRSAkey(std::string logFile);

		/**
		*@brief Get PM with read-write mount
		*/
		EXPORT std::string getWritablePM();

		/**
		*@brief Get PM with read-write mount
		*/
		EXPORT std::string getHotStandbyPM();

		/**
		*@brief Get PM with read-write mount
		*/
		EXPORT void setHotStandbyPM(std::string moduleName);

		/**
		*@brief Distribute erydb Configure File
		*/
		EXPORT void distributeConfigFile(std::string name = "system", std::string file = "erydb.xml");

		/**
		*@brief Switch Parent OAM Module 
		*  Return true if we need to wait for systme restart 
		*/
		EXPORT bool switchParentOAMModule(std::string moduleName, GRACEFUL_FLAG gracefulflag);

		/**
		*@brief Get Storage Config Data
		*/
		EXPORT systemStorageInfo_t getStorageConfig();

		/**
		*@brief Get PM - DBRoot Config data
		*/
		EXPORT void getPmDbrootConfig(const int pmid, DBRootConfigList& dbrootconfiglist);

		/**
		*@brief Get DBRoot - PM Config data
		*/
		EXPORT void getDbrootPmConfig(const int dbrootid, int& pmid);

		EXPORT void getDbrootPmConfig(const int dbrootid, std::string& pmid);

		/**
		*@brief Get System DBRoot Config data
		*/
		EXPORT void getSystemDbrootConfig(DBRootConfigList& dbrootconfiglist);

		/**
		*@brief Set PM - DBRoot Config data
		*/
		EXPORT void setPmDbrootConfig(const int pmid, DBRootConfigList& dbrootconfiglist);

		/**
		*@brief Manual Move PM - DBRoot data
		*/
		EXPORT void manualMovePmDbroot(std::string residePM, std::string dbrootIDs, std::string toPM);

		/**
		*@brief Auto Move PM - DBRoot data
		*/
		EXPORT bool autoMovePmDbroot(std::string residePM);

		/**
		*@brief Auto Un-Move PM - DBRoot data
		*/
		EXPORT bool autoUnMovePmDbroot(std::string toPM);

		/**
		*@brief add DBRoot
		*/
		EXPORT void addDbroot(const int dbrootNumber, DBRootConfigList& dbrootlist, std::string EBSsize = oam::UnassignedName);

		/**
		*@brief add UM Disk
		*/
		EXPORT void addUMdisk(const int moduleID, std::string& volumeName, std::string& device, std::string EBSsize = oam::UnassignedName);

		/**
		*@brief distribute Fstab Updates
		*/
		EXPORT void distributeFstabUpdates(std::string entry, std::string toPM = "system" );

		/**
		*@brief assign DBRoot
		*/
		EXPORT void assignDbroot(std::string toPM, DBRootConfigList& dbrootlist);

		/**
		*@brief unassign DBRoot
		*/
		EXPORT void unassignDbroot(std::string residePM, DBRootConfigList& dbrootlist);

		/**
		*@brief get unassigned DBRoot list
		*/
		EXPORT void getUnassignedDbroot(DBRootConfigList& dbrootlist);

		/**
		*@brief remove DBRoot
		*/
		EXPORT void removeDbroot(DBRootConfigList& dbrootlist);

		/**
		*@brief get AWS Device Name for DBRoot ID
		*/
		EXPORT storageID_t getAWSdeviceName( const int dbrootid);

		/**
		*@brief set System DBRoot Count
		*/
		EXPORT void setSystemDBrootCount();

		/**
		*@brief set FilesPerColumnPartition based on value of old
		* FilePerColumnPartition and old DbRootCount that is given
		*/
		EXPORT void setFilesPerColumnPartition( int oldDbRootCount );

            /** @brief send Device Notification Msg
             */
            EXPORT int sendDeviceNotification(std::string deviceName, NOTIFICATION_TYPE type, std::string payload = "");

            /** @brief run DBHealth Check
             */
 			EXPORT void checkDBFunctional(bool action = true);

            /** @brief mysql-erydb service command
             */
 			EXPORT void actionMysqlerydb(MYSQLERYDB_ACTION action);

            /** @brief validate Module name
             */
            EXPORT int validateModule(const std::string name);

            /** @brief getEC2LocalInstance
             */
            EXPORT std::string getEC2LocalInstance(std::string name = "dummy");

            /** @brief getEC2LocalInstanceType
             */
            EXPORT std::string getEC2LocalInstanceType(std::string name = "dummy");

            /** @brief getEC2LocalInstanceSubnet
             */
            EXPORT std::string getEC2LocalInstanceSubnet(std::string name = "dummy");

            /** @brief launchEC2Instance
             */
            EXPORT std::string launchEC2Instance(const std::string name = "dummy", const std::string IPAddress = oam::UnassignedName, const std::string type = oam::UnassignedName, const std::string group = oam::UnassignedName);

            /** @brief getEC2InstanceIpAddress
             */
            EXPORT std::string getEC2InstanceIpAddress(std::string instanceName);

            /** @brief terminateEC2Instance
             */
            EXPORT void terminateEC2Instance(std::string instanceName);

            /** @brief stopEC2Instance
             */
            EXPORT void stopEC2Instance(std::string instanceName);

            /** @brief startEC2Instance
             */
            EXPORT bool startEC2Instance(std::string instanceName);

            /** @brief assignElasticIP
             */
            EXPORT bool assignElasticIP(std::string instanceName, std::string IpAddress);

            /** @brief deassignElasticIP
             */
            EXPORT bool deassignElasticIP(std::string IpAddress);

            /** @brief createEC2Volume
             */
            EXPORT std::string createEC2Volume(std::string size, std::string name = "dummy");

            /** @brief getEC2VolumeStatus
             */
            EXPORT std::string getEC2VolumeStatus(std::string volumeName);

            /** @brief attachEC2Volume
             */
            EXPORT bool attachEC2Volume(std::string volumeName, std::string deviceName,std::string instanceName);

            /** @brief detachEC2Volume
             */
            EXPORT bool detachEC2Volume(std::string volumeName);

            /** @brief deleteEC2Volume
             */
            EXPORT bool deleteEC2Volume(std::string volumeName);

            /** @brief createEC2tag
             */
            EXPORT bool createEC2tag(std::string resourceName, std::string tagName, std::string tagValue);

		/**
		*@brief  take action on Syslog process
		*/
		EXPORT void syslogAction( std::string action);

		/**
		*@brief  call dbrm control
		*/
		EXPORT void dbrmctl(std::string command);

		/** @brief Wait for system to close transactions
		 *  
		 *  When a Shutdown, stop, restart or suspend operation is
		 *  requested but there are active transactions of some sort,
		 *  We wait for all transactions to close before performing
		 *  the action.
		 */
		EXPORT bool waitForSystem(PROC_MGT_MSG_REQUEST request, messageqcpp::IOSocket& ios, messageqcpp::ByteStream& stillWorkingMsg);

		void amazonReattach(std::string toPM, dbrootList dbrootConfigList, bool attach = false);
		void mountDBRoot(dbrootList dbrootConfigList, bool mount = true);

		/**
		*@brief  gluster control
		*/
		EXPORT int glusterctl(GLUSTER_COMMANDS command, std::string argument1, std::string& argument2, std::string& errmsg);

		/**
		* @brief	changeMyCnf
		*
		* purpose:	change my.cnf
		*
		**/
		EXPORT bool changeMyCnf( std::string paramater, std::string value );

		/**
		* @brief	enableMySQLRep
		*
		* purpose:	enable MySQL Replication on the system
		*
		**/
		EXPORT bool enableMySQLRep( std::string password );

		/**
		* @brief	diableMySQLRep
		*
		* purpose:	disable MySQL Replication on the system
		*
		**/
		EXPORT bool disableMySQLRep();

		/** @brief check Gluster Log after a Gluster control call
		*/
		EXPORT int checkGlusterLog(std::string logFile, std::string& errmsg);

		/** @brief check and get mysql user password
		*/
		EXPORT std::string getMySQLPassword(bool bypassConfig = false);

		/** @brief update fstab with dbroot mounts
		*/
		EXPORT std::string updateFstab(std::string device, std::string dbrootID);

		/**
		* @brief Write the message to the log
		*/
		static void writeLog(const std::string logContent, const logging::LOG_TYPE logType = logging::LOG_TYPE_INFO);

		bool checkSystemRunning();

    private:

	    int sendMsgToProcMgr3(messageqcpp::ByteStream::byte requestType, alarmmanager::AlarmList& alarmlist, const std::string date);

            /** @brief build and send request message to Process Manager
             */
            int sendMsgToProcMgr(messageqcpp::ByteStream::byte requestType, const std::string name = "",
                GRACEFUL_FLAG gracefulflag = FORCEFUL, ACK_FLAG ackflag = ACK_NO,
                const std::string argument1 = "", const std::string argument2 = "", int timeout = 600);

            /** @brief build and send request message to Process Manager 2
             */
    		int sendMsgToProcMgr2(messageqcpp::ByteStream::byte requestType, DeviceNetworkList devicenetworklist,
        		GRACEFUL_FLAG gracefulflag, ACK_FLAG ackflag, const std::string password = oam::UnassignedName, const std::string mysqlpw = oam::UnassignedName);

            /** @brief build and send request message to Process Manager
             *  Check for status messages
             */
            int sendMsgToProcMgrWithStatus(messageqcpp::ByteStream::byte requestType, const std::string name = "",
                GRACEFUL_FLAG gracefulflag = GRACEFUL, ACK_FLAG ackflag = ACK_YES,
                const std::string argument1 = "", const std::string argument2 = "", int timeout = 600);

            // check for Ack message from Process Manager
            //	int checkMsgFromProcMgr(messageqcpp::ByteStream::byte requestType, const std::string name);

            /** @brief validate Process name
             */
    		int validateProcess(const std::string moduleName, std::string processName);

            /** @brief send status updates to process monitor
             */
    		void sendStatusUpdate(messageqcpp::ByteStream obs, messageqcpp::ByteStream::byte returnRequestType);

			std::string erydbConfigFile;
			std::string AlarmConfigFile;
			std::string ProcessConfigFile;
			std::string InstallDir;
			static int  UseHdfs;

    };	// end of class

}	// end of namespace

namespace procheartbeat
{

    class ProcHeartbeat
    {
        public:
            /** @brief ProcHeartbeat Class constructor
             */
            ProcHeartbeat();

            /** @brief ProcHeartbeat Class destructor
             */
            virtual ~ProcHeartbeat();

            /** @brief Register for Proc Heartbeat
             *
             */
            void registerHeartbeat(int ID = 1);

            /** @brief De-Register for Proc Heartbeat
             *
             * DeregisterHeartbeat
             */
            void deregisterHeartbeat(int ID = 1);

            /** @brief Send Proc Heartbeat
             *
             */
            void sendHeartbeat(int ID = 1, oam::ACK_FLAG ackFlag = oam::ACK_NO);
	};
}

#undef EXPORT

#endif
// vim:ts=4 sw=4:

