/* Copyright (C) 2014 EryDB, Inc.
   Copyright (C) 2016 MariaDB Corporation

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
   * $Id: postConfigure.cpp 64 2006-10-12 22:21:51Z dhill $
   *
   *
   * List of files being updated by post-install configure:
   *		erydb/etc/erydb.xml
   *		erydb/etc/ProcessConfig.xml
   *		/etc/rc.local
   *
   ******************************************************************************************/
   /**
    * @file
    */

#include <unistd.h>
#include <iterator>
#include <numeric>
#include <deque>
#include <iostream>
#include <ostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <limits.h>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/sysinfo.h>
#include <climits>
#include <cstring>
#include <glob.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>

#include <string.h> /* for strncpy */
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

#include <readline/readline.h>
#include <readline/history.h>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/tokenizer.hpp"

#include "liboamcpp.h"
#include "configcpp.h"

using namespace std;
using namespace oam;
using namespace config;

#include "helpers.h"
using namespace installer;

#include "license.h"

typedef struct DBRoot_Module_struct {
    std::string		moduleName;
    std::string     dbrootID;
} DBRootModule;

typedef std::vector<DBRootModule> DBRootList;

typedef struct Performance_Module_struct {
    std::string     moduleIP1;
    std::string     moduleIP2;
    std::string     moduleIP3;
    std::string     moduleIP4;
} PerformanceModule;

typedef std::vector<PerformanceModule> PerformanceModuleList;

void offLineAppCheck();
bool setOSFiles(string parentOAMModuleName, string DBRootStorageType);
bool checkSaveConfigFile();
string getModuleName();
bool setModuleName(string moduleName);
bool updateBash();
bool makeModuleFile(string moduleName, string parentOAMModuleName);
bool updateProcessConfig(int serverTypeInstall);
bool uncommenterydbXml(string entry);
bool makeRClocal(string moduleType, string moduleName, int IserverTypeInstall);
bool createMetaDataDir(string DBRootStorageType);
bool pkgCheck(std::string erydbPackage);
bool storageSetup(bool amazonInstall);
void setSystemName();
bool copyFstab(string moduleName);
bool attachVolume(string instanceName, string volumeName, string deviceName, string dbrootPath);

void remoteInstallThread(void *);

void setSingleServerInstall(bool startOfflinePrompt);
bool singleServerDBrootSetup();


typedef struct ModuleIP_struct {
    std::string     IPaddress;
    std::string     moduleName;
} ModuleIP;

std::string launchInstance(ModuleIP moduleip);

string erydbPackage;
//string erydbPackage2;
//string erydbPackage3;
//string mysqlPackage;
//string mysqldPackage;

string parentOAMModuleName;
int pmNumber = 0;
int umNumber = 0;

string DBRootStorageLoc;
string DBRootStorageType;
string UMStorageType;

string PMVolumeSize = oam::UnassignedName;
string UMVolumeSize = oam::UnassignedName;
string UMVolumeType = "standard";
string PMVolumeType = "standard";
string PMVolumeIOPS = oam::UnassignedName;
string UMVolumeIOPS = oam::UnassignedName;


int DBRootCount;
string deviceName;

Config* sysConfig = Config::makeConfig();
string SystemSection = "SystemConfig";
string InstallSection = "Installation";
string ModuleSection = "SystemModuleConfig";
string serverTypeInstall;
int    IserverTypeInstall;
string parentOAMModuleIPAddr;
string parentOAMModuleHostName;

string remote_installer_debug = "1";
bool thread_remote_installer = true;

string singleServerInstall = "1";
string reuseConfig = "n";
string oldFileName;
string glusterCopies;
string glusterInstalled = "n";
string hadoopInstalled = "n";
string mysqlPort = oam::UnassignedName;
string systemName;

bool noPrompting = false;
bool rootUser = true;
string USER = "root";
bool hdfs = false;
bool gluster = false;
bool pmwithum = false;
bool mysqlRep = false;
string MySQLRep = "n";
string PMwithUM = "n";
bool amazonInstall = false;

string DataFileEnvFile;

string installDir;
string HOME = "/root";

extern string pwprompt;
string mysqlpw = " ";

extern const char* pcommand;
extern string prompt;
string localHostName;
string password;


/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
    std::string command;
} thread_data_t;

 
int main(int argc, char *argv[]) {
    Oam oam;
    ChildModuleList childmodulelist;
    ChildModuleList niclist;
    ChildModule childmodule;
    DBRootModule dbrootmodule;
    DBRootList dbrootlist;
    PerformanceModuleList performancemodulelist;
    int DBRMworkernodeID = 0;
    string nodeps = "-h";
    bool startOfflinePrompt = false;
    noPrompting = false;
    string cmd;
    //  	struct sysinfo myinfo; 
        // hidden options
        // -f for force use nodeps on rpm install
        // -o to prompt for process to start offline
    string systemLang = "C";
    setlocale(LC_ALL, systemLang.c_str());
    setenv("LANG", "C", 1);
    //default
    installDir = installDir + "";
    //see if we can determine our own location
    ostringstream oss;
    oss << "/proc/" << getpid() << "/exe";
    ssize_t rlrc;
    const size_t psz = PATH_MAX;
    char thisexepath[psz + 1];
    memset(thisexepath, 0, psz + 1);
    rlrc = readlink(oss.str().c_str(), thisexepath, psz);
    if (rlrc > 0) {
        thisexepath[rlrc] = 0;
        //should look something like '/usr/local/erydb/bin/postConfigure'
        char* ptr;
        ptr = strrchr(thisexepath, '/');
        if (ptr) {
            *ptr = 0;
            ptr = strrchr(thisexepath, '/');
            if (ptr) {
                *ptr = 0;
                installDir = thisexepath;
            }
        }
    }
    //check if root-user
    int user;
    user = getuid();
    if (user != 0)
        rootUser = false;

    char* p = getenv("USER");
    if (p && *p)
        USER = p;

    if (!rootUser) {
        char* p = getenv("HOME");
        if (p && *p)
            HOME = p;
    }
    // parse params
    for (int i = 1; i < argc; i++) {
        if (string("-h") == argv[i]) {
            cout << endl;
            cout << "This is the erydb System Configuration and Installation tool." << endl;
            cout << "It will Configure the erydb System based on Operator inputs and" << endl;
            cout << "will perform a Package Installation of all of the Modules within the" << endl;
            cout << "System that is being configured." << endl;
            cout << endl;
            cout << "IMPORTANT: This tool should only be run on a Performance Module Server," << endl;
            cout << "           preferably Module #1" << endl;
            cout << endl;
            cout << "Instructions:" << endl << endl;
            cout << "	Press 'enter' to accept a value in (), if available or" << endl;
            cout << "	Enter one of the options within [], if available, or" << endl;
            cout << "	Enter a new value" << endl << endl;
            cout << endl;
            cout << "Usage: postConfigure [-h][-c][-u][-p][-s][-port][-i]" << endl;
            cout << "   -h  Help" << endl;
            cout << "   -c  Config File to use to extract configuration data, default is erydb.xml.rpmsave" << endl;
            cout << "   -u  Upgrade, Install using the Config File from -c, default to erydb.xml.rpmsave" << endl;
            cout << "	    If ssh-keys aren't setup, you should provide passwords as command line arguments" << endl;
            cout << "   -p  Unix Password, used with no-prompting option" << endl;
            cout << "   -s  Single Threaded Remote Install" << endl;
            cout << "   -n  no prompting" << endl;
            cout << "   -port mysql Port Address" << endl;
            cout << "   -i Non-root Install directory, Only use for non-root installs" << endl;
            exit(0);
        } else if (string("-s") == argv[i])
            thread_remote_installer = false;
        else if (string("-f") == argv[i])
            nodeps = "--nodeps";
        else if (string("-o") == argv[i])
            startOfflinePrompt = true;
        else if (string("-c") == argv[i]) {
            i++;
            if (i >= argc) {
                cout << "   ERROR: Config File not provided" << endl;
                exit(1);
            }
            oldFileName = argv[i];
            if (oldFileName.find("erydb.xml") == string::npos) {
                cout << "   ERROR: Config File is not a erydb.xml file name" << endl;
                exit(1);
            }
        } else if (string("-p") == argv[i]) {
            i++;
            if (i >= argc) {
                cout << "   ERROR: Password not provided" << endl;
                exit(1);
            }
            password = argv[i];
            if (password.find("-") != string::npos) {
                cout << "   ERROR: Valid Password not provided" << endl;
                exit(1);
            }
        } else if (string("-u") == argv[i])
            noPrompting = true;
        // for backward compatibility
        else if (string("-n") == argv[i])
            noPrompting = true;
        else if (string("-port") == argv[i]) {
            i++;
            if (i >= argc) {
                cout << "   ERROR: erydb Port ID not supplied" << endl;
                exit(1);
            }
            mysqlPort = argv[i];
            if (atoi(mysqlPort.c_str()) < 1000 || atoi(mysqlPort.c_str()) > 9999) {
                cout << "   ERROR: Invalid erydb Port ID supplied, must be between 1000-9999" << endl;
                exit(1);
            }
        } else if (string("-i") == argv[i]) {
            i++;
            if (i >= argc) {
                cout << "   ERROR: Path not provided" << endl;
                exit(1);
            }
            installDir = argv[i];
        } else {
            cout << "   ERROR: Invalid Argument = " << argv[i] << endl;
            cout << "   Usage: postConfigure [-h][-c][-u][-p][-s][-port][-i]" << endl;
            exit(1);
        }
    }

    if (installDir[0] != '/') {
        cout << "   ERROR: Install dir '" << installDir << "' is not absolute" << endl;
        exit(1);
    }

    if (oldFileName == "")
        oldFileName = installDir + "/etc/erydb.xml.rpmsave";
    //out tips
    {
        cout << endl;
        cout << "This is the erydb System Configuration and Installation tool." << endl;
        cout << "It will Configure the erydb System and will perform a Package" << endl;
        cout << "Installation of all of the Servers within the System that is being configured." << endl;
        cout << endl;

        cout << "IMPORTANT: This tool should only be run on the Parent OAM Module" << endl;
        cout << "           which is a Performance Module, preferred Module #1" << endl;
        cout << endl;
    }

    if (!noPrompting) {
        cout << "Prompting instructions:" << endl << endl;
        cout << "	Press 'enter' to accept a value in (), if available or" << endl;
        cout << "	Enter one of the options within [], if available, or" << endl;
        cout << "	Enter a new value" << endl << endl;
    } else {
        //get current time and date
        time_t now;
        now = time(NULL);
        struct tm tm;
        localtime_r(&now, &tm);
        char timestamp[200];
        strftime(timestamp, 200, "%m:%d:%y-%H:%M:%S", &tm);
        string currentDate = timestamp;

        string postConfigureLog = "/var/log/erydb-postconfigure-" + currentDate;

        cout << "With the no-Prompting Option being specified, you will be required to have the following:" << endl;
        cout << endl;
        cout << " 1. Root user ssh keys setup between all nodes in the system or" << endl;
        cout << "    use the password command line option." << endl;
        cout << " 2. A Configure File to use to retrieve configure data, default to erydb.xml.rpmsave" << endl;
        cout << "    or use the '-c' option to point to a configuration file." << endl;
        cout << endl;
        //		cout << " Output if being redirected to " << postConfigureLog << endl;

        //		redirectStandardOutputToFile(postConfigureLog, false );
    }

    //check if erydb is up and running
    if (oam.checkSystemRunFlag() && oam.checkSystemRunning()) {
        cout << "EryDB is running, can't run postConfigure while EryDB is running. Exiting.." << endl;
        exit(0);
    }

    //determine package type
    string EEPackageType = "binary";
    if (!rootUser) {
        EEPackageType = "binary";
    } else {
        int rtnCode = system("rpm -qi erydb > /tmp/erydb.txt 2>&1");
        if (WEXITSTATUS(rtnCode) == 0) {
            EEPackageType = "rpm";
        } else {
            rtnCode = system("dpkg -s erydb > /tmp/erydb.txt 2>&1");
            if (WEXITSTATUS(rtnCode) == 0)
                EEPackageType = "deb";
            else
                EEPackageType = "binary";
        }
    }
    try {
        sysConfig->setConfig(InstallSection, "EEPackageType", EEPackageType);
    } catch (...) {
        cout << "ERROR: Problem setting EEPackageType from the erydb System Configuration file" << endl;
        exit(1);
    }

    //if binary install, then run post-install just in case the user didnt run it
    if (EEPackageType == "binary") {
        //run post install
        cmd = installDir + "/bin/post-install --installdir=" + installDir + " > /dev/null 2>&1";
        system(cmd.c_str());
    }
    {
        string ermsg;
        if (!License::checkLicense(ermsg)) {
            cout << "ERROR:" << ermsg << endl;
            exit(1);
        }
    }
    //check Config saved files
    if (!checkSaveConfigFile()) {//update reuseConfig
        cout << "ERROR: Configuration File not setup" << endl;
        exit(1);
    }
    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        exit(1);
    }

    //check for local ip address as pm1  去掉pm1限制后可以删除此判断
    ModuleConfig moduleconfig;
    try {
        oam.getSystemConfig("pm1", moduleconfig);
        if (moduleconfig.hostConfigList.size() > 0) {
            HostConfigList::iterator pt1 = moduleconfig.hostConfigList.begin();
            string PM1ipAdd = (*pt1).IPAddr;
            //cout << PM1ipAdd << endl;

            if (PM1ipAdd != "127.0.0.1" && PM1ipAdd != "0.0.0.0") {
                struct ifaddrs *ifap, *ifa;
                struct sockaddr_in *sa;
                char *addr;
                bool found = false;
                getifaddrs(&ifap);
                for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
                    if (ifa->ifa_addr->sa_family == AF_INET) {
                        sa = (struct sockaddr_in *) ifa->ifa_addr;
                        addr = inet_ntoa(sa->sin_addr);
                        //printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
                        if (PM1ipAdd == addr) {//match
                            found = true;
                        }
                    }
                    if (found)
                        break;
                }
                freeifaddrs(ifap);
                if (!found) {
                    cout << endl;
                    cout << "ERROR: postConfigure install can only be done on the PM1" << endl;
                    cout << "designated node. The configured PM1 IP address doesn't match the local" << endl;
                    cout << "IP Address. exiting..." << endl;
                    exit(1);
                }
            }
        }
    } catch (...) {
    }
    // run my.cnf upgrade script
    if (reuseConfig == "y") {
        cmd = installDir + "/bin/mycnfUpgrade  > /tmp/mycnfUpgrade.log 2>&1";
        int rtnCode = system(cmd.c_str());
        if (WEXITSTATUS(rtnCode) != 0)
            cout << "Error: Problem upgrade my.cnf, check /tmp/mycnfUpgrade.log" << endl;
        else
            cout << "NOTE: my.cnf file was upgraded based on my.cnf.rpmsave" << endl;
    }

    //check mysql port changes
    string MySQLPort;
    try {
        MySQLPort = sysConfig->getConfig(InstallSection, "MySQLPort");
    } catch (...) {
    }

    if (mysqlPort == oam::UnassignedName) {
        if (MySQLPort.empty() || MySQLPort == "") {
            mysqlPort = "3306";
            try {
                sysConfig->setConfig(InstallSection, "MySQLPort", "3306");
            } catch (...) {
            }
        } else
            mysqlPort = MySQLPort;
    } else {	// mysql port was providing on command line
        try {
            sysConfig->setConfig(InstallSection, "MySQLPort", mysqlPort);
        } catch (...) {
        }
    }

    try {
        oam.setSystemConfig("MySQLPasswordConfig", oam::UnassignedName);
    } catch (...) {}
    // set is SingleServerInstall
    //out tips 
    {
    cout << endl;
    cout << "===== Setup System Server Type Configuration =====" << endl << endl;
    cout << "There are 2 options when configuring the System Server Type: single and multi" << endl << endl;
    cout << "  'single'  - Single-Server install is used when there will only be 1 server configured" << endl;
    cout << "              on the system. It can also be used for production systems, if the plan is" << endl;
    cout << "              to stay single-server." << endl << endl;
    cout << "  'multi'   - Multi-Server install is used when you want to configure multiple servers now or" << endl;
    cout << "              in the future. With Multi-Server install, you can still configure just 1 server" << endl;
    cout << "              now and add on addition servers/modules in the future." << endl << endl;
    }
    {
        string temp;
        try {
            temp = sysConfig->getConfig(InstallSection, "SingleServerInstall");
        } catch (...) {
        }
        if (temp == "y")
            singleServerInstall = "1";
        else
            singleServerInstall = "2";
        while (true) {
            prompt = "Select the type of System Server install [1=single, 2=multi] (" + singleServerInstall + ") > ";
            pcommand = callReadline(prompt.c_str());
            string temp;
            if (pcommand) {
                if (strlen(pcommand) > 0)
                    temp = pcommand;
                else
                    temp = singleServerInstall;
                callFree(pcommand);
                if (temp == "1") {
                    setSingleServerInstall(startOfflinePrompt);
                } else if (temp == "2") {
                    singleServerInstall = temp;
                    break;
                }
                cout << "Invalid Entry, please re-enter" << endl;
                if (noPrompting)
                    exit(1);
                continue;
            }
            break;
        }
        try {
            sysConfig->setConfig(InstallSection, "SingleServerInstall", "n");
        } catch (...) {
            cout << "ERROR: Problem setting SingleServerInstall from the erydb System Configuration file" << endl;
            exit(1);
        }

        if (!writeConfig(sysConfig)) {
            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
            exit(1);
        }

    }
    //
    // Multi-server install
    //ouput tips
    {
        cout << endl;
        //cleanup/create local/etc  directory
        cmd = "rm -rf " + installDir + "/local/etc > /dev/null 2>&1";
        system(cmd.c_str());
        cmd = "mkdir " + installDir + "/local/etc > /dev/null 2>&1";
        system(cmd.c_str());
        cout << endl << "===== Setup System Module Type Configuration =====" << endl << endl;
        cout << "There are 2 options when configuring the System Module Type: separate and combined" << endl << endl;
        cout << "  'separate' - User and Performance functionality on separate servers." << endl << endl;
        cout << "  'combined' - User and Performance functionality on the same server" << endl << endl;
    }
    // set ServerTypeInstall
    try {
        serverTypeInstall = sysConfig->getConfig(InstallSection, "ServerTypeInstall");
    } catch (...) {
        cout << "ERROR: Problem getting ServerTypeInstall from the erydb System Configuration file" << endl;
        exit(1);
    }
    while (true) {
        prompt = "Select the type of System Module Install [1=separate, 2=combined] (" + serverTypeInstall + ") > ";
        pcommand = callReadline(prompt.c_str());
        cout << endl;
        if (pcommand) {
            if (strlen(pcommand) > 0) serverTypeInstall = pcommand;
            callFree(pcommand);
        }
        if (serverTypeInstall != "1" && serverTypeInstall != "2") {
            cout << "Invalid Entry, please re-enter" << endl << endl;
            if (noPrompting)
                exit(1);
            continue;
        }
        break;
    }
    IserverTypeInstall = atoi(serverTypeInstall.c_str());
    // set Server Type Installation Indicator
    try {
        sysConfig->setConfig(InstallSection, "ServerTypeInstall", serverTypeInstall);
    } catch (...) {
        cout << "ERROR: Problem setting ServerTypeInstall in the erydb System Configuration file" << endl;
        exit(1);
    }
    // set local query flag
    switch (IserverTypeInstall) {
        case (oam::INSTALL_COMBINE_DM_UM_PM):	// combined #1 - dm/um/pm on a single server
        {
            cout << "Combined Server Installation will be performed." << endl;
            cout << "The Server will be configured as a Performance Module." << endl;
            cout << "All erydb Processes will run on the Performance Modules." << endl << endl;
            //module ProcessConfig.xml to setup all apps on the dm
            if (!updateProcessConfig(IserverTypeInstall))
                cout << "Update ProcessConfig.xml error" << endl;
            //store local query flag
            try {
                sysConfig->setConfig(InstallSection, "PMwithUM", "n");
            } catch (...) {
            }
            pmwithum = false;
            break;
        }
        default:	// normal, separate UM and PM
        {
            cout << "Seperate Server Installation will be performed." << endl << endl;
            try {
                PMwithUM = sysConfig->getConfig(InstallSection, "PMwithUM");
            } catch (...) {
            }

            if (PMwithUM == "y")
                pmwithum = true;
            string answer = "n";
            cout << "NOTE: Local Query Feature allows the ability to query data from a single Performance" << endl;
            cout << "      Module. Check erydb Admin Guide for additional information." << endl << endl;
            while (true) {
                if (pmwithum)
                    prompt = "Local Query feature is Enabled, Do you want to disable? [y,n] (n) > ";
                else
                    prompt = "Enable Local Query feature? [y,n] (n) > ";

                pcommand = callReadline(prompt.c_str());
                if (pcommand) {
                    if (strlen(pcommand) > 0) answer = pcommand;
                    callFree(pcommand);
                }

                if (answer == "y" || answer == "n") {
                    cout << endl;
                    break;
                } else
                    cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                if (noPrompting)
                    exit(1);
            }

            if (pmwithum) {
                if (answer == "y") {
                    pmwithum = false;
                    PMwithUM = "n";
                }
            } else {
                if (answer == "y") {
                    pmwithum = true;
                    PMwithUM = "y";
                }
            }
            try {
                sysConfig->setConfig(InstallSection, "PMwithUM", PMwithUM);
            } catch (...) {
            }
            break;
        }
    } 

    // check for Schema Schema is Local Query wasnt selected mysqlRep ,now use galera wsrep
    {
        try {
            MySQLRep = "n";
            mysqlRep = false;
            sysConfig->setConfig(InstallSection, "MySQLRep", MySQLRep);
        } catch (...) {
        }
        if (!writeConfig(sysConfig)) {
            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
            exit(1);
        }
    }

    //amazon install setup check
    bool amazonInstall = false;
    string cloud = oam::UnassignedName;
    {
        int rtnCode = system("which aws  2>/dev/null");
        if (WEXITSTATUS(rtnCode) == 0) {
            system("aws --version > /tmp/amazon.log 2>&1");
            ifstream in("/tmp/amazon.log");
            in.seekg(0, std::ios::end);
            int size = in.tellg();
            if (size == 0 || oam.checkLogStatus("/tmp/amazon.log", "not installed"))
                amazonInstall = false;
            else
                amazonInstall = true;
        }
        try {
            cloud = sysConfig->getConfig(InstallSection, "Cloud");
        } catch (...) {
            cloud = oam::UnassignedName;
        }
        if (cloud == "disable") {
            amazonInstall = false;
        }
        if (amazonInstall) {
            if (cloud == oam::UnassignedName) {
                cout << "NOTE: Amazon AWS CLI Tools are installed and allow erydb to create Instances and ABS Volumes" << endl << endl;
                while (true) {
                    string enable = "y";
                    prompt = "Do you want to have EryDB use the Amazon AWS CLI Tools [y,n] (y) > ";
                    pcommand = callReadline(prompt.c_str());

                    if (pcommand) {
                        if (strlen(pcommand) > 0) enable = pcommand;
                        callFree(pcommand);

                        if (enable == "n") {
                            amazonInstall = false;

                            try {
                                sysConfig->setConfig(InstallSection, "Cloud", "disable");
                            } catch (...) {
                            };

                            break;
                        }
                    }
                    if (enable != "y") {
                        cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                        if (noPrompting)
                            exit(1);
                    } else {
                        try {
                            sysConfig->setConfig(InstallSection, "Cloud", "amazon-vpc");
                        } catch (...) {
                        }
                    }

                    break;
                }
            } else
                cout << "NOTE: Configured to have EryDB use the Amazon AWS CLI Tools" << endl << endl;
            if (amazonInstall) {
                string cmd = installDir + "/bin/ERYDBgetCredentials.sh >/dev/null 2>&1";
                int rtnCode = system(cmd.c_str());
                if (WEXITSTATUS(rtnCode) != 0) {
                    cout << endl << "Error: No IAM Profile with Security Certificates used or AWS CLI Certificate file configured" << endl;
                    cout << "Check Amazon Install Documenation for additional information, exiting..." << endl;
                    exit(1);
                }
            }
            if (!writeConfig(sysConfig)) {
                cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                exit(1);
            }
        }

        if (pmwithum) {
            cout << endl << "NOTE: Local Query Feature is enabled" << endl;
        }
        //Write out Updated System Configuration File
        try {
            sysConfig->setConfig(InstallSection, "InitialInstallFlag", "n");
        } catch (...) {
            cout << "ERROR: Problem setting InitialInstallFlag from the erydb System Configuration file" << endl;
            exit(1);
        }

        if (!writeConfig(sysConfig)) {
            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
            exit(1);
        }
    }
    cout << endl;
    // prompt for system name
    setSystemName();
    cout << endl;
    //parentOAMModuleName check
    string parentOAMModuleType;
    string localModuleName;
    int parentOAMModuleID = 0;
    {
        try {
            oamModuleInfo_t t;
            t = oam.getModuleInfo();
            localModuleName = boost::get<0>(t);
        } catch (exception& e) {}

        //get Parent OAM Module Name , on init set parentOAMModuleName=pm1
        parentOAMModuleName = "pm1";
        try {
            parentOAMModuleName = sysConfig->getConfig(SystemSection, "ParentOAMModuleName");
        } catch (...) {
        }
        if (parentOAMModuleName != localModuleName || parentOAMModuleName != "pm1") {
            string answer = "n";
            while (true) {
                prompt = "Do you want to reset to create a new cluster? [y,n] (n) > ";
                pcommand = callReadline(prompt.c_str());
                if (pcommand) {
                    if (strlen(pcommand) > 0) answer = pcommand;
                    callFree(pcommand);
                }
                if (answer == "y" || answer == "n") {
                    cout << endl;
                    break;
                } else {
                    cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                }
            }
            if (answer == "y") {
                localModuleName = "pm1";
                parentOAMModuleName = "pm1";
                string fileName = installDir + "/local/module";
                string cmd = "echo " + parentOAMModuleName + " > " + fileName;
                system(cmd.c_str());
            }
        }
        //if (localModuleName != parentOAMModuleName) {
        //    cout << endl << endl << "ERROR: exiting, postConfigure can only run executed on " + parentOAMModuleName + ", current module is " << localModuleName << endl;
        //    exit(1);
        //}
        //try {
        //    sysConfig->setConfig(SystemSection, "ParentOAMModuleName", parentOAMModuleName);
        //} catch (...) {
        //    cout << "ERROR: Problem setting ParentOAMModuleName the erydb System Configuration file" << endl;
        //    exit(1);
        //}
        ////all others both StandbyOAMModuleName 
        //// set Standby Parent OAM module and IP to unassigned
        //try {
        //    sysConfig->setConfig(SystemSection, "StandbyOAMModuleName", oam::UnassignedName);
        //    sysConfig->setConfig("ProcStatusControlStandby", "IPAddr", oam::UnassignedIpAddr);
        //} catch (...) {
        //    cout << "ERROR: Problem setting ParentStandbyOAMModuleName the erydb System Configuration file" << endl;
        //    exit(1);
        //}

        //create associated local/etc directory for parentOAMModuleName
        cmd = "mkdir " + installDir + "/local/etc/" + parentOAMModuleName + " > /dev/null 2>&1";
        system(cmd.c_str());
        //setup local module file name
        if (!makeModuleFile(parentOAMModuleName, parentOAMModuleName)) {
            cout << "makeModuleFile error" << endl;
            exit(1);
        }
        cout << endl;

        if (startOfflinePrompt)
            offLineAppCheck();

        parentOAMModuleType = parentOAMModuleName.substr(0, MAX_MODULE_TYPE_SIZE);
        parentOAMModuleID = atoi(parentOAMModuleName.substr(MAX_MODULE_TYPE_SIZE, MAX_MODULE_ID_SIZE).c_str());
        if (parentOAMModuleID < 1) {
            cout << "ERROR: Invalid Module ID of less than 1" << endl;
            exit(1);
        }
    }
    //Get list of configured system modules
    SystemModuleTypeConfig sysModuleTypeConfig;
    try {
        oam.getSystemConfig(sysModuleTypeConfig);
    } catch (...) {
        cout << "ERROR: Problem reading the erydb System Configuration file" << endl;
        exit(1);
    }

    // setup storage
    if (!storageSetup(amazonInstall)) {
        cout << "ERROR: Problem setting up storage" << endl;
        exit(1);
    }
    if (hdfs || !rootUser) {
        if (!updateBash())
            cout << "updateBash error" << endl;
    }
    // setup memory paramater settings
    cout << endl << "===== Setup Memory Configuration =====" << endl << endl;
    switch (IserverTypeInstall) {
        case (oam::INSTALL_COMBINE_DM_UM_PM):	// combined #1 - dm/um/pm on a single server
        {
            // are we using settings from previous config file?
            if (reuseConfig == "n") {
                if (!uncommenterydbXml("NumBlocksPct")) {
                    cout << "Update erydb.xml NumBlocksPct Section" << endl;
                    exit(1);
                }
                string numBlocksPct;
                try {
                    numBlocksPct = sysConfig->getConfig("DBBC", "NumBlocksPct");
                } catch (...) {
                }
                if (numBlocksPct == "70" || numBlocksPct.empty()) {
                    numBlocksPct = "50";
                    if (hdfs)
                        numBlocksPct = "25";
                }
                try {
                    sysConfig->setConfig("DBBC", "NumBlocksPct", numBlocksPct);
                    cout << endl << "NOTE: Setting 'NumBlocksPct' to " << numBlocksPct << "%" << endl;
                } catch (...) {
                    cout << "ERROR: Problem setting NumBlocksPct in the erydb System Configuration file" << endl;
                    exit(1);
                }
                string percent = "25%";
                if (hdfs) {
                    percent = "12%";
                }
                cout << "      Setting 'TotalUmMemory' to " << percent << endl;
                try {
                    sysConfig->setConfig("HashJoin", "TotalUmMemory", percent);
                } catch (...) {
                    cout << "ERROR: Problem setting TotalUmMemory in the erydb System Configuration file" << endl;
                    exit(1);
                }
                if (!writeConfig(sysConfig)) {
                    cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                    exit(1);
                }
            } else {
                try {
                    string numBlocksPct = sysConfig->getConfig("DBBC", "NumBlocksPct");
                    cout << endl;
                    if (numBlocksPct.empty())
                        cout << "NOTE: Using the default setting for 'NumBlocksPct' at 70%" << endl;
                    else
                        cout << "NOTE: Using previous configuration setting for 'NumBlocksPct' = " << numBlocksPct << "%" << endl;
                    string totalUmMemory = sysConfig->getConfig("HashJoin", "TotalUmMemory");
                    cout << "      Using previous configuration setting for 'TotalUmMemory' = " << totalUmMemory << endl;
                } catch (...) {
                    cout << "ERROR: Problem reading NumBlocksPct/TotalUmMemory in the erydb System Configuration file" << endl;
                    exit(1);
                }
            }
            break;
        }
        default:	// normal, separate UM and PM
        {
            // are we using settings from previous config file?
            if (reuseConfig == "n") {
                string numBlocksPct;
                try {
                    numBlocksPct = sysConfig->getConfig("DBBC", "NumBlocksPct");
                } catch (...) {
                }
                if (numBlocksPct.empty()) {
                    numBlocksPct = "70";
                    if (hdfs)
                        numBlocksPct = "35";
                }
                try {
                    sysConfig->setConfig("DBBC", "NumBlocksPct", numBlocksPct);

                    cout << "NOTE: Setting 'NumBlocksPct' to " << numBlocksPct << "%" << endl;
                } catch (...) {
                    cout << "ERROR: Problem setting NumBlocksPct in the erydb System Configuration file" << endl;
                    exit(1);
                }
                string percent = "50%";
                if (hdfs) {
                    percent = "25%";
                }
                cout << "      Setting 'TotalUmMemory' to " << percent << endl;
                try {
                    sysConfig->setConfig("HashJoin", "TotalUmMemory", percent);
                } catch (...) {
                    cout << "ERROR: Problem setting TotalUmMemory in the erydb System Configuration file" << endl;
                    exit(1);
                }
                if (!writeConfig(sysConfig)) {
                    cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                    exit(1);
                }
            } else {
                try {
                    string numBlocksPct = sysConfig->getConfig("DBBC", "NumBlocksPct");
                    if (numBlocksPct.empty())
                        cout << "NOTE: Using the default setting for 'NumBlocksPct' at 70%" << endl;
                    else
                        cout << "NOTE: Using previous configuration setting for 'NumBlocksPct' = " << numBlocksPct << "%" << endl;
                    string totalUmMemory = sysConfig->getConfig("HashJoin", "TotalUmMemory");
                    cout << "      Using previous configuration setting for 'TotalUmMemory' = " << totalUmMemory << endl;
                } catch (...) {
                    cout << "ERROR: Problem reading NumBlocksPct/TotalUmMemory in the erydb System Configuration file" << endl;
                    exit(1);
                }
            }
            break;
        }
    }
    //Write out Updated System Configuration File
    {
        try {
            sysConfig->setConfig(InstallSection, "InitialInstallFlag", "y");
        } catch (...) {
            cout << "ERROR: Problem setting InitialInstallFlag from the erydb System Configuration file" << endl;
            exit(1);
        }
        if (!writeConfig(sysConfig)) {
            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
            exit(1);
        }
    }

    // Module Configuration
    cout << endl;
    cout << "===== Setup the Module Configuration =====" << endl << endl;
    if (amazonInstall) {
        cout << "Amazon Install: For Module Configuration, you have the option to provide the" << endl;
        cout << "existing Instance IDs or have the Instances created." << endl;
        cout << "You will be prompted during the Module Configuration setup section." << endl;
    }
    //get OAM Parent Module IP addresses and Host Name, if it exist
    for (unsigned int i = 0; i < sysModuleTypeConfig.moduletypeconfig.size(); i++) {
        DeviceNetworkList::iterator listPT = sysModuleTypeConfig.moduletypeconfig[i].ModuleNetworkList.begin();
        for (; listPT != sysModuleTypeConfig.moduletypeconfig[i].ModuleNetworkList.end(); listPT++) {
            HostConfigList::iterator pt1 = (*listPT).hostConfigList.begin();
            if ((*listPT).DeviceName == parentOAMModuleName) {
                parentOAMModuleIPAddr = (*pt1).IPAddr;
                parentOAMModuleHostName = (*pt1).HostName;
                break;
            }
        }
    }

    unsigned int maxPMNicCount = 1;
    //configure module type
    bool parentOAMmoduleConfig = false;
    bool moduleSkip = false;
    char hostname[128];
    gethostname(hostname, sizeof hostname);
    localHostName = hostname;
    string procMon_port=sysConfig->getConfig("localhost_ProcessMonitor", "Port");
    for (unsigned int i = 0; i < sysModuleTypeConfig.moduletypeconfig.size(); i++) {
        string moduleType = sysModuleTypeConfig.moduletypeconfig[i].ModuleType;
        string moduleDesc = sysModuleTypeConfig.moduletypeconfig[i].ModuleDesc;
        int moduleCount = sysModuleTypeConfig.moduletypeconfig[i].ModuleCount;
        //set dm count to 0
        if (moduleType == "dm") {
            moduleCount = 0;
            try {
                string ModuleCountParm = "ModuleCount" + oam.itoa(i + 1);
                sysConfig->setConfig(ModuleSection, ModuleCountParm, oam.itoa(moduleCount));
                continue;
            } catch (...) {
            }
        }
        //set module count
        int oldModuleCount = moduleCount;
        //verify and setup of modules count set count to 0
        switch (IserverTypeInstall) {
        case (oam::INSTALL_COMBINE_DM_UM_PM):
        {
            if (moduleType == "um") {
                moduleCount = 0;
                try {
                    string ModuleCountParm = "ModuleCount" + oam.itoa(i + 1);
                    sysConfig->setConfig(ModuleSection, ModuleCountParm, oam.itoa(moduleCount));
                    continue;
                } catch (...) {
                    cout << "ERROR: Problem setting Module Count in the erydb System Configuration file" << endl;
                    exit(1);
                }
            }
            break;
        }
        default:
            break;
        }
        //set module count
        cout << endl << "----- " << moduleDesc << " Configuration -----" << endl << endl;
        while (true) {
            int licSize = oam::MAX_MODULE;
            if (moduleType == "pm") {
                licSize = License::getPmCountLimit();
            } else if (moduleType == "um") {
                licSize = License::getUmCountLimit();
            }
            prompt = "Enter number of " + moduleDesc + "s [1," + oam.itoa(licSize) + "] (" + oam.itoa(oldModuleCount) + ") > ";
            moduleCount = oldModuleCount;
            pcommand = callReadline(prompt.c_str());
            if (pcommand) {
                if (strlen(pcommand) > 0) moduleCount = atoi(pcommand);
                callFree(pcommand);
            }
            if (moduleCount < 1 || moduleCount > licSize) {
                cout << endl << "ERROR: Invalid Module Count '" + oam.itoa(moduleCount) + "', please re-enter" << endl << endl;
                if (noPrompting)exit(1);
                continue;
            }
            if (parentOAMModuleType == moduleType && moduleCount == 0) {
                cout << endl << "ERROR: Parent OAM Module Type is '" + parentOAMModuleType + "', so you have to have at least 1 of this Module Type, please re-enter" << endl << endl;
                if (noPrompting)exit(1);
                continue;
            }
            //update count
            try {
                string ModuleCountParm = "ModuleCount" + oam.itoa(i + 1);
                sysConfig->setConfig(ModuleSection, ModuleCountParm, oam.itoa(moduleCount));
                break;
            } catch (...) {
                cout << "ERROR: Problem setting Module Count in the erydb System Configuration file" << endl;
                exit(1);
            }
        }
         
        //set internal replica size
        if (DBRootStorageType == "internal" && moduleType == "pm") {
            int licSize = oam::MAX_MODULE;
            licSize = License::getPmCountLimit();
            string oldRepSize = "3";
            try {
                oldRepSize = sysConfig->getConfig(SystemSection, "PMreplicateCount");
            } catch (...) {
                oldRepSize = "3";
            }
            int PMreplicateSize = atoi(oldRepSize.c_str());
            if(PMreplicateSize<1)
                PMreplicateSize=1;
            if (PMreplicateSize > moduleCount) {
                PMreplicateSize = moduleCount;
            }
            int maxRep=MAX_DATA_REPLICATESIZE;
            if(moduleCount<maxRep)
                maxRep=moduleCount;
            while (true) {
                prompt = "Enter number of PM storage copies [1-" + oam.itoa(maxRep) + "] (" + oam.itoa(PMreplicateSize) + ") > ";
                pcommand = callReadline(prompt.c_str());
                if (pcommand) {
                    if (strlen(pcommand) > 0) PMreplicateSize = atoi(pcommand);
                    callFree(pcommand);
                }
                if (PMreplicateSize >= 1) {
                    cout << endl;
                    break;
                } else {
                    cout << endl << "ERROR: Invalid replicate Count '" + oam.itoa(PMreplicateSize) + "', please re-enter" << endl << endl;
                }
            }
            try {
                sysConfig->setConfig(SystemSection, "PMreplicateCount", oam.itoa(PMreplicateSize));
            } catch (...) {
                cout << "ERROR: Problem setting PMreplicateCount in the erydb System Configuration file" << endl;
                exit(1);
            }
            if (!writeConfig(sysConfig)) {
                cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                exit(1);
            }
        }

        if (moduleType == "pm") {
            pmNumber = moduleCount;
            //chekck internal replica size
            if (DBRootStorageType == "internal") {
                string oldRepSize = "3";
                try {
                    oldRepSize = sysConfig->getConfig(SystemSection, "PMreplicateCount");
                } catch (...) {
                    cout << "ERROR: Problem getting PMreplicateCount in the erydb System Configuration file" << endl;
                    exit(1);
                }
                int PMreplicateSize = atoi(oldRepSize.c_str());
                if (pmNumber < PMreplicateSize) {
                    try {
                        cout << "warning:replicateCount["<<PMreplicateSize<<"] lage than PM count["<<pmNumber <<"],set replicateCount to PM count " << endl;
                        PMreplicateSize = pmNumber;
                        // sysConfig->setConfig(SystemSection, "PMreplicateCount", oam.itoa(PMreplicateSize));
                    } catch (...) {
                        // cout << "ERROR: Problem setting PMreplicateCount in the erydb System Configuration file" << endl;
                        // exit(1);
                    }
                } 
            }
        }
        if (moduleType == "um")
            umNumber = moduleCount;
        int moduleID = 1;
        int listSize = sysModuleTypeConfig.moduletypeconfig[i].ModuleNetworkList.size();
        //clear any Equipped Module IP addresses that aren't in current ID range
        for (int j = 0; j < listSize; j++) {
            for (unsigned int k = 1; k < MAX_NIC + 1; k++) {
                //<!--主机ID 网口id 类型ID-->
                string ModuleIPAddr = "ModuleIPAddr" + oam.itoa(j + 1) + "-" + oam.itoa(k) + "-" + oam.itoa(i + 1);
                if (!(sysConfig->getConfig(ModuleSection, ModuleIPAddr).empty())) {
                    if (j + 1 < moduleID || j + 1 > moduleID + (moduleCount - 1)) {
                        string ModuleHostName = "ModuleHostName" + oam.itoa(j + 1) + "-" + oam.itoa(k) + "-" + oam.itoa(i + 1);
                        sysConfig->setConfig(ModuleSection, ModuleIPAddr, oam::UnassignedIpAddr);
                        sysConfig->setConfig(ModuleSection, ModuleHostName, oam::UnassignedName);
                    }
                }
            }
        }

        //configure module name
        string newModuleHostName;
        while (true) {
            int saveModuleID = moduleID;
            string moduleDisableState;
            int enableModuleCount = moduleCount;
            for (int k = 0; k < moduleCount; k++, moduleID++) {
                string newModuleName = moduleType + oam.itoa(moduleID);

                if (moduleType == parentOAMModuleType && moduleID != parentOAMModuleID && !parentOAMmoduleConfig) {
                    //skip this module for now, need to configure parent OAM Module first
                    moduleSkip = true;
                    continue;
                }

                if (moduleType == parentOAMModuleType && moduleID == parentOAMModuleID && parentOAMmoduleConfig)
                    //skip, aleady configured
                    continue;

                if (moduleType == parentOAMModuleType && moduleID == parentOAMModuleID && !parentOAMmoduleConfig)
                    parentOAMmoduleConfig = true;

                string moduleNameDesc = moduleDesc + " #" + oam.itoa(moduleID);
                PerformanceModule performancemodule;

                if (newModuleName == parentOAMModuleName)
                    cout << endl << "*** Parent OAM Module " << moduleNameDesc << " Configuration ***" << endl << endl;
                else
                    cout << endl << "*** " << moduleNameDesc << " Configuration ***" << endl << endl;

                moduleDisableState = oam::ENABLEDSTATE;

                //setup HostName/IPAddress for each NIC
                for (unsigned int nicID = 1; nicID < MAX_NIC + 1; nicID++) {
                    string moduleHostName = oam::UnassignedName;
                    string moduleIPAddr = oam::UnassignedIpAddr;
                    //find exists config nic ip and hostname
                    DeviceNetworkList::iterator listPT = sysModuleTypeConfig.moduletypeconfig[i].ModuleNetworkList.begin();
                    for (; listPT != sysModuleTypeConfig.moduletypeconfig[i].ModuleNetworkList.end(); listPT++) {
                        if (newModuleName == (*listPT).DeviceName) {
                            if (nicID == 1) {
                                moduleDisableState = (*listPT).DisableState;
                                if (moduleDisableState.empty() ||
                                    moduleDisableState == oam::UnassignedName ||
                                    moduleDisableState == oam::AUTODISABLEDSTATE)
                                    moduleDisableState = oam::ENABLEDSTATE;
                            }

                            HostConfigList::iterator pt1 = (*listPT).hostConfigList.begin();
                            for (; pt1 != (*listPT).hostConfigList.end(); pt1++) {
                                if ((*pt1).NicID == nicID) {
                                    moduleHostName = (*pt1).HostName;
                                    moduleIPAddr = (*pt1).IPAddr;
                                    break;
                                }
                            }
                        }
                    }
                    //set module host state
                    if (nicID == 1) {
                        if (moduleDisableState != oam::ENABLEDSTATE) {
                            string disabled = "y";
                            while (true) {
                                if (enableModuleCount > 1) {
                                    prompt = "Module '" + newModuleName + "' is Disabled, do you want to leave it Disabled? [y,n] (y) > ";
                                    pcommand = callReadline(prompt.c_str());
                                    if (pcommand) {
                                        if (strlen(pcommand) > 0) disabled = pcommand;
                                        callFree(pcommand);
                                    }
                                    if (disabled == "y" || disabled == "n") {
                                        cout << endl;
                                        break;
                                    } else
                                        cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                                    if (noPrompting)
                                        exit(1);
                                    disabled = "y";
                                } else {
                                    string enable = "y";
                                    cout << "Module '" + newModuleName + "' is Disabled. It needs to be enabled to startup erydb." << endl;
                                    prompt = "Do you want to Enable it or exit? [y,exit] (y) > ";
                                    pcommand = callReadline(prompt.c_str());
                                    if (pcommand) {
                                        if (strlen(pcommand) > 0) enable = pcommand;
                                        callFree(pcommand);
                                    }
                                    if (enable == "y") {
                                        disabled = "n";
                                        break;
                                    } else {
                                        if (enable == "exit") {
                                            cout << "Exiting postConfigure..." << endl;
                                            exit(1);
                                        } else
                                            cout << "Invalid Entry, please enter 'y' for yes or 'exit'" << endl;

                                        if (noPrompting)
                                            exit(1);
                                        enable = "y";
                                    }
                                }
                            }

                            if (disabled == "n")
                                moduleDisableState = oam::ENABLEDSTATE;
                            else
                                enableModuleCount--;
                        }

                        //reset Module Disable State
                        string moduleDisableStateParm = "ModuleDisableState" + oam.itoa(moduleID) + "-" + oam.itoa(i + 1);
                        try {
                            sysConfig->setConfig(ModuleSection, moduleDisableStateParm, moduleDisableState);
                        } catch (...) {
                            cout << "ERROR: Problem setting ModuleDisableState in the erydb System Configuration file" << endl;
                            exit(1);
                        }
                        if (!writeConfig(sysConfig)) {
                            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                            exit(1);
                        }

                        //skip configuration if if DISABLED state
                        if (moduleDisableState != oam::ENABLEDSTATE)
                            break;
                    }

                    bool moduleHostNameFound = true;
                    if (moduleHostName.empty()) {
                        moduleHostNameFound = true;
                        moduleHostName = oam::UnassignedName;
                    }
                    if (moduleIPAddr.empty())
                        moduleIPAddr = oam::UnassignedIpAddr;
                    string newModuleIPAddr;
                    while (true) {
                        newModuleHostName = moduleHostName;
                        //可调用脚本动态创建虚拟机
                        if (amazonInstall) {
                            if (moduleHostName == oam::UnassignedName &&
                                newModuleName == "pm1" && nicID == 1) {
                                //get local instance name (pm1)
                                string localInstance = oam.getEC2LocalInstance();
                                if (localInstance == "failed" || localInstance.empty() || localInstance == "") {
                                    moduleHostName = oam::UnassignedName;
                                    prompt = "Enter EC2 Instance ID (" + moduleHostName + ") > ";
                                } else {
                                    newModuleHostName = localInstance;
                                    cout << "EC2 Instance ID for pm1: " + localInstance << endl;
                                    prompt = "";
                                }
                            } else {
                                if (moduleHostName == oam::UnassignedName) {
                                    //check if need to create instance or user enter ID
                                    string create = "y";
                                    while (true) {
                                        pcommand = callReadline("Create Instance for " + newModuleName + " [y,n] (y) > ");
                                        if (pcommand) {
                                            if (strlen(pcommand) > 0) create = pcommand;
                                            callFree(pcommand);
                                        }
                                        if (create == "y" || create == "n")
                                            break;
                                        else
                                            cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                                        create = "y";
                                        if (noPrompting)
                                            exit(1);
                                    }
                                    if (create == "y") {
                                        ModuleIP moduleip;
                                        moduleip.moduleName = newModuleName;

                                        string AmazonVPCNextPrivateIP = "autoassign";
                                        try {
                                            oam.getSystemConfig("AmazonVPCNextPrivateIP", AmazonVPCNextPrivateIP);
                                        } catch (...) {}

                                        moduleip.IPaddress = AmazonVPCNextPrivateIP;

                                        newModuleHostName = launchInstance(moduleip);
                                        if (newModuleHostName == oam::UnassignedName) {
                                            cout << "launch Instance failed for " + newModuleName << endl;
                                            exit(1);
                                        }
                                        prompt = "";
                                    } else
                                        prompt = "Enter EC2 Instance ID (" + moduleHostName + ") > ";
                                } else
                                    prompt = "Enter EC2 Instance ID (" + moduleHostName + ") > ";
                            }
                        } else {
                            if (moduleHostName == oam::UnassignedName &&
                                newModuleName == "pm1" && nicID == 1) { 
                                moduleHostName = localHostName;
                            } 
                            prompt = "Enter Nic Interface #" + oam.itoa(nicID) + " Host Name (" + moduleHostName + ") > ";
                        }
                        if (prompt != "") {
                            pcommand = callReadline(prompt.c_str());
                            if (pcommand) {
                                if (strlen(pcommand) > 0)
                                    newModuleHostName = pcommand;
                                else
                                    newModuleHostName = moduleHostName;
                                callFree(pcommand);
                            }
                        }
                        if (newModuleHostName == oam::UnassignedName && nicID == 1) {
                            cout << "Invalid Entry, please re-enter" << endl;
                            if (noPrompting)
                                exit(1);
                        } else {
                            if (newModuleHostName != oam::UnassignedName) {
                                //check and see if hostname already used
                                bool matchFound = false;
                                ChildModuleList::iterator list1 = niclist.begin();
                                for (; list1 != niclist.end(); list1++) {
                                    if (newModuleHostName == (*list1).hostName) {
                                        cout << "Invalid Entry, already assigned to '" + (*list1).moduleName + "', please re-enter" << endl;
                                        matchFound = true;
                                        if (noPrompting)
                                            exit(1);
                                        break;
                                    }
                                }
                                if (matchFound)
                                    continue;

                                //check Instance ID and get IP Address if running
                                if (amazonInstall) {
                                    string instanceType = oam.getEC2LocalInstanceType(newModuleHostName);
                                    if (moduleType == "pm") {
                                        try {
                                            sysConfig->setConfig(InstallSection, "PMInstanceType", instanceType);
                                        } catch (...) {
                                        }
                                    } else {
                                        try {
                                            sysConfig->setConfig(InstallSection, "UMInstanceType", instanceType);
                                        } catch (...) {
                                        }
                                    }

                                    cout << "Getting Private IP Address for Instance " << newModuleHostName << ", please wait..." << endl;
                                    newModuleIPAddr = oam.getEC2InstanceIpAddress(newModuleHostName);
                                    if (newModuleIPAddr == "stopped") {
                                        cout << "ERROR: Instance " + newModuleHostName + " not running, please start and hit 'enter'" << endl << endl;
                                        if (noPrompting)
                                            exit(1);
                                        continue;
                                    } else {
                                        if (newModuleIPAddr == "terminated") {
                                            cout << "ERROR: Instance " + newModuleHostName + " doesn't have an Private IP Address, retrying" << endl << endl;
                                            if (noPrompting)
                                                exit(1);
                                            continue;
                                        } else {
                                            cout << "Private IP Address of " + newModuleHostName + " is " + newModuleIPAddr << endl << endl;

                                            moduleIPAddr = newModuleIPAddr;
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                            break;
                        }
                    }

                    //set New Module Host Name
                    string moduleHostNameParm = "ModuleHostName" + oam.itoa(moduleID) + "-" + oam.itoa(nicID) + "-" + oam.itoa(i + 1);
                    try {
                        sysConfig->setConfig(ModuleSection, moduleHostNameParm, newModuleHostName);
                    } catch (...) {
                        cout << "ERROR: Problem setting Host Name in the erydb System Configuration file" << endl;
                        exit(1);
                    }

                    if (newModuleHostName == oam::UnassignedName)
                        newModuleIPAddr = oam::UnassignedIpAddr;
                    else {
                        if (!amazonInstall) {
                            if (moduleIPAddr == oam::UnassignedIpAddr) {
                                //get IP Address
                                string IPAddress = oam.getIPAddress(newModuleHostName);
                                if (!IPAddress.empty())
                                    newModuleIPAddr = IPAddress;
                                else
                                    newModuleIPAddr = oam::UnassignedIpAddr;
                            } else
                                newModuleIPAddr = moduleIPAddr;

                            if (newModuleIPAddr == "127.0.0.1")
                                newModuleIPAddr = "unassigned";

                            //prompt for IP address
                            while (true) {
                                prompt = "Enter Nic Interface #" + oam.itoa(nicID) + " IP Address of " + newModuleHostName + " (" + newModuleIPAddr + ") > ";
                                pcommand = callReadline(prompt.c_str());
                                if (pcommand) {
                                    if (strlen(pcommand) > 0) newModuleIPAddr = pcommand;
                                    callFree(pcommand);
                                }

                                if (newModuleIPAddr == "127.0.0.1" || newModuleIPAddr == "0.0.0.0" || newModuleIPAddr == "128.0.0.1") {
                                    cout << endl << newModuleIPAddr + " is an Invalid IP Address for a multi-server system, please re-enter" << endl << endl;
                                    newModuleIPAddr = "unassigned";
                                    if (noPrompting)
                                        exit(1);
                                    continue;
                                }
                                //check ip 
                                if (oam.isValidIP(newModuleIPAddr)) {
                                    //check and see if hostname already used
                                    bool matchFound = false;
                                    ChildModuleList::iterator list1 = niclist.begin();
                                    for (; list1 != niclist.end(); list1++) {
                                        if (newModuleIPAddr == (*list1).moduleIP) {
                                            cout << "Invalid Entry, IP Address already assigned to '" + (*list1).moduleName + "', please re-enter" << endl;
                                            matchFound = true;
                                            if (noPrompting)
                                                exit(1);
                                            break;
                                        }
                                    }
                                    if (matchFound)
                                        continue;
                                    // run ping test to validate
                                    string cmdLine = "ping ";
                                    string cmdOption = " -c 1 -w 5 >> /dev/null";
                                    string cmd = cmdLine + newModuleIPAddr + cmdOption;
                                    int rtnCode = system(cmd.c_str());
                                    if (WEXITSTATUS(rtnCode) != 0) {
                                        //NIC failed to respond to ping
                                        string temp = "2";
                                        while (true) {
                                            cout << endl;
                                            prompt = "IP Address of '" + newModuleIPAddr + "' failed ping test, please validate. Do you want to continue or re-enter [1=continue, 2=re-enter] (2) > ";
                                            if (noPrompting)
                                                exit(1);
                                            pcommand = callReadline(prompt.c_str());
                                            if (pcommand) {
                                                if (strlen(pcommand) > 0) temp = pcommand;
                                                callFree(pcommand);
                                            }

                                            if (temp == "1" || temp == "2")
                                                break;
                                            else {
                                                temp = "2";
                                                cout << endl << "Invalid entry, please re-enter" << endl;
                                                if (noPrompting)
                                                    exit(1);
                                            }
                                        }
                                        cout << endl;
                                        if (temp == "1")
                                            break;
                                    } else{// good ping
                                        string IPAddress = oam.getIPAddress(newModuleHostName);
                                        if (IPAddress != newModuleIPAddr) {
                                            string cmd = "sed -i -e '/"+ newModuleHostName +"/d' -e '/" + newModuleIPAddr + "/d'  -e /^[[:space:]]*$/d /etc/hosts ";
                                            system(cmd.c_str());
                                            cmd = "echo '" + newModuleIPAddr + " " + newModuleHostName + "' >> /etc/hosts ";
                                            system(cmd.c_str());
                                        }
                                        break;
                                    }
                                } else {
                                    cout << endl << "Invalid IP Address format, xxx.xxx.xxx.xxx, please re-enter" << endl << endl;
                                    if (noPrompting)
                                        exit(1);
                                }
                            }
                        }
                    }

                    //set Module IP address
                    string moduleIPAddrNameParm = "ModuleIPAddr" + oam.itoa(moduleID) + "-" + oam.itoa(nicID) + "-" + oam.itoa(i + 1);
                    try {
                        sysConfig->setConfig(ModuleSection, moduleIPAddrNameParm, newModuleIPAddr);
                    } catch (...) {
                        cout << "ERROR: Problem setting IP address in the erydb System Configuration file" << endl;
                        exit(1);
                    }

                    if (newModuleHostName == oam::UnassignedName && moduleHostNameFound)
                        // exit out to next module ID
                        break;

                    if (moduleType == "pm" && moduleDisableState == oam::ENABLEDSTATE) {
                        switch (nicID) {
                        case 1:
                            performancemodule.moduleIP1 = newModuleIPAddr;
                            break;
                        case 2:
                            performancemodule.moduleIP2 = newModuleIPAddr;
                            break;
                        case 3:
                            performancemodule.moduleIP3 = newModuleIPAddr;
                            break;
                        case 4:
                            performancemodule.moduleIP4 = newModuleIPAddr;
                            break;
                        }
                        if (maxPMNicCount < nicID)
                            maxPMNicCount = nicID;
                    }

                    //save Nic host name and IP
                    childmodule.moduleName = newModuleName;
                    childmodule.moduleIP = newModuleIPAddr;
                    childmodule.hostName = newModuleHostName;
                    niclist.push_back(childmodule);

                    if (nicID > 1)
                        continue;

                    //save Child modules
                    if (newModuleName != parentOAMModuleName) {
                        childmodule.moduleName = newModuleName;
                        childmodule.moduleIP = newModuleIPAddr;
                        childmodule.hostName = newModuleHostName;
                        childmodulelist.push_back(childmodule);
                    }

                    //set port addresses
                    if (newModuleName == parentOAMModuleName) {
                        parentOAMModuleHostName = newModuleHostName;
                        parentOAMModuleIPAddr = newModuleIPAddr;

                        //set Parent Processes Port IP Address
                        string parentProcessMonitor = parentOAMModuleName + "_ProcessMonitor";
                        sysConfig->setConfig(parentProcessMonitor, "IPAddr", parentOAMModuleIPAddr);
                        sysConfig->setConfig(parentProcessMonitor, "Port", procMon_port);
                        sysConfig->setConfig("ProcMgr", "IPAddr", parentOAMModuleIPAddr);
                        //sysConfig->setConfig("ProcHeartbeatControl", "IPAddr", parentOAMModuleIPAddr);
                        sysConfig->setConfig("ProcStatusControl", "IPAddr", parentOAMModuleIPAddr);
                        string parentServerMonitor = parentOAMModuleName + "_ServerMonitor";
                        sysConfig->setConfig(parentServerMonitor, "IPAddr", parentOAMModuleIPAddr);
                        sysConfig->setConfig(parentServerMonitor, "Port", "8622");
                        string portName = parentOAMModuleName + "_WriteEngineServer";
                        sysConfig->setConfig(portName, "IPAddr", parentOAMModuleIPAddr);
                        sysConfig->setConfig(portName, "Port", "8630");

                        //set User Module's IP Addresses
                        if (IserverTypeInstall == oam::INSTALL_COMBINE_DM_UM_PM) {
                            string Section = "ExeMgr" + oam.itoa(moduleID);
                            sysConfig->setConfig(Section, "IPAddr", newModuleIPAddr);
                            sysConfig->setConfig(Section, "Port", "8601");
                            sysConfig->setConfig(Section, "Module", parentOAMModuleName);
                            //set Performance Module's IP's to first NIC IP entered
                            sysConfig->setConfig("DDLProc", "IPAddr", newModuleIPAddr);
                            sysConfig->setConfig("DMLProc", "IPAddr", newModuleIPAddr);
                        }
                        //set User Module's IP Addresses
                        if (pmwithum) {
                            string Section = "ExeMgr" + oam.itoa(moduleID + umNumber);
                            sysConfig->setConfig(Section, "IPAddr", newModuleIPAddr);
                            sysConfig->setConfig(Section, "Port", "8601");
                            sysConfig->setConfig(Section, "Module", parentOAMModuleName);
                        }
                        //setup rc.local file in module tmp dir
                        if (!makeRClocal(moduleType, newModuleName, IserverTypeInstall))
                            cout << "makeRClocal error" << endl;
                    } else {
                        //set child Process Monitor Port IP Address
                        string portName = newModuleName + "_ProcessMonitor";
                        sysConfig->setConfig(portName, "IPAddr", newModuleIPAddr);
                        sysConfig->setConfig(portName, "Port", procMon_port);
                        //set child Server Monitor Port IP Address
                        portName = newModuleName + "_ServerMonitor";
                        sysConfig->setConfig(portName, "IPAddr", newModuleIPAddr);
                        sysConfig->setConfig(portName, "Port", "8622");

                        //set Performance Module WriteEngineServer Port IP Address
                        if (moduleType == "pm") {
                            portName = newModuleName + "_WriteEngineServer";
                            sysConfig->setConfig(portName, "IPAddr", newModuleIPAddr);
                            sysConfig->setConfig(portName, "Port", "8630");
                        }

                        //set User Module's IP Addresses
                        if (moduleType == "um" ||
                            (moduleType == "pm" && IserverTypeInstall == oam::INSTALL_COMBINE_DM_UM_PM) ||
                            /* (moduleType == "um" && IserverTypeInstall == oam::INSTALL_COMBINE_DM_UM) ||
                            (moduleType == "pm" && IserverTypeInstall == oam::INSTALL_COMBINE_PM_UM)||*/ 
                            (moduleType == "pm" && pmwithum)) {

                            string Section = "ExeMgr" + oam.itoa(moduleID);
                            if (moduleType == "pm" && pmwithum)
                                Section = "ExeMgr" + oam.itoa(moduleID + umNumber);

                            sysConfig->setConfig(Section, "IPAddr", newModuleIPAddr);
                            sysConfig->setConfig(Section, "Port", "8601");
                            sysConfig->setConfig(Section, "Module", newModuleName);
                        }

                        //set Performance Module's IP's to first NIC IP entered
                        if (newModuleName == "um1") {
                            sysConfig->setConfig("DDLProc", "IPAddr", newModuleIPAddr);
                            sysConfig->setConfig("DMLProc", "IPAddr", newModuleIPAddr);
                        }
                    }

                    if (!writeConfig(sysConfig)) {
                        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                        exit(1);
                    }

                    //create associated local/etc directory for module
                    string cmd = "mkdir " + installDir + "/local/etc/" + newModuleName + " > /dev/null 2>&1";
                    system(cmd.c_str()); 
                    if (!makeModuleFile(newModuleName, parentOAMModuleName))
                        cout << "makeModuleFile error" << endl;
                    if (newModuleHostName == localHostName) {
                        string cmd = "echo '"+ newModuleName +"' > "+installDir + "/local/module";
                        system(cmd.c_str());
                    } else if (newModuleName == parentOAMModuleName) {
                        string cmd = "scp -r " + installDir + "/local/ " + parentOAMModuleIPAddr + ":" + installDir + "/> /dev/null 2>&1";
                        system(cmd.c_str());
                        cmd = "echo '" + newModuleName + "' > /tmp/module";
                        system(cmd.c_str());
                        cmd = "scp /tmp/module " + parentOAMModuleIPAddr + ":" + installDir + "/local/ > /dev/null 2>&1";
                        system(cmd.c_str());
                    } 
                    //setup rc.local file in module tmp dir
                    if (!makeRClocal(moduleType, newModuleName, IserverTypeInstall))
                        cout << "makeRClocal error" << endl;

                    //setup DBRM Processes
                    if (newModuleName == parentOAMModuleName)
                        sysConfig->setConfig("DBRM_Controller", "IPAddr", newModuleIPAddr);

                    if (moduleDisableState == oam::ENABLEDSTATE) {
                        DBRMworkernodeID++;
                        string DBRMSection = "DBRM_Worker" + oam.itoa(DBRMworkernodeID);
                        sysConfig->setConfig(DBRMSection, "IPAddr", newModuleIPAddr);
                        sysConfig->setConfig(DBRMSection, "Module", newModuleName);
                    }

                    // only support 1 nic ID per Amazon instance
                    if (amazonInstall)
                        break;

                } //end of nicID loop

                //enter storage for user module
                if (moduleType == "um" && moduleDisableState == oam::ENABLEDSTATE) {
                    //get EC2 volume name and info
                    if (UMStorageType == "external" && amazonInstall &&
                        IserverTypeInstall != oam::INSTALL_COMBINE_DM_UM_PM) {

                        string volumeNameID = "UMVolumeName" + oam.itoa(moduleID);
                        string volumeName = oam::UnassignedName;
                        string deviceNameID = "UMVolumeDeviceName" + oam.itoa(moduleID);
                        string deviceName = oam::UnassignedName;

                        // prompt for volume ID
                        try {
                            volumeName = sysConfig->getConfig(InstallSection, volumeNameID);
                            deviceName = sysConfig->getConfig(InstallSection, deviceNameID);
                        } catch (...) {
                            string volumeName = oam::UnassignedName;
                        }

                        string create = "n";
                        if (volumeName == oam::UnassignedName || volumeName.empty() || volumeName == "") {
                            create = "y";

                            while (true) {
                                pcommand = callReadline("Create an EBS volume for " + newModuleName + " ?  [y,n] (y) > ");
                                {
                                    if (strlen(pcommand) > 0) create = pcommand;
                                    callFree(pcommand);
                                }
                                if (create == "y" || create == "n")
                                    break;
                                else
                                    cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                                create = "y";
                                if (noPrompting)
                                    exit(1);
                            }
                        }

                        if (create == "n") {
                            prompt = "Enter Volume ID assigned to module '" + newModuleName + "' (" + volumeName + ") > ";
                            pcommand = callReadline(prompt.c_str());
                            if (pcommand) {
                                if (strlen(pcommand) > 0) volumeName = pcommand;
                                callFree(pcommand);
                            }

                            //get device name based on DBRoot ID
                            deviceName = "/dev/sdf";
                        } else {
                            //create new UM volume
                            try {
                                oam.addUMdisk(moduleID, volumeName, deviceName, UMVolumeSize);
                            } catch (...) {
                                cout << "ERROR: volume create failed for " + newModuleName << endl;
                                return false;
                            }
                        }

                        //write volume and device name
                        try {
                            sysConfig->setConfig(InstallSection, volumeNameID, volumeName);
                            sysConfig->setConfig(InstallSection, deviceNameID, deviceName);
                        } catch (...) {
                            cout << "ERROR: Problem setting Volume/Device Names in the erydb System Configuration file" << endl;
                            return false;
                        }

                        if (!writeConfig(sysConfig)) {
                            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                            exit(1);
                        }

                        string pathID = installDir + "/mysql/db";

                        // check volume for attach and try to attach if not
                        if (!attachVolume(newModuleHostName, volumeName, deviceName, pathID)) {
                            cout << "attachVolume error" << endl;
                            exit(1);
                        }
                    }
                }

                //if upgrade, get list of configure dbroots
                DBRootConfigList dbrootConfigList;
                if (reuseConfig == "y") {
                    try {
                        oam.getSystemDbrootConfig(dbrootConfigList);
                    } catch (...) {}
                }

                //enter dbroots for performance module
                if (moduleType == "pm" && moduleDisableState == oam::ENABLEDSTATE) {
                    //get number of dbroots
                    string moduledbrootcount = "ModuleDBRootCount" + oam.itoa(moduleID) + "-" + oam.itoa(i + 1);
                    string tempCount;
                    try {
                        tempCount = sysConfig->getConfig(ModuleSection, moduledbrootcount);
                    } catch (...) {
                        cout << "ERROR: Problem setting DBRoot count in the erydb System Configuration file" << endl;
                        exit(1);
                    }

                    unsigned int count = 0;
                    if (tempCount != oam::UnassignedName)
                        count = atoi(tempCount.c_str());

                    string dbrootList;
                    for (unsigned int id = 1; id < count + 1; ) {
                        string moduledbrootid = "ModuleDBRootID" + oam.itoa(moduleID) + "-" + oam.itoa(id) + "-" + oam.itoa(i + 1);
                        try {
                            string dbrootid = sysConfig->getConfig(ModuleSection, moduledbrootid);
                            if (dbrootid != oam::UnassignedName) {
                                sysConfig->setConfig(ModuleSection, moduledbrootid, oam::UnassignedName);
                                dbrootList = dbrootList + dbrootid;
                                id++;
                                if (id < count + 1)
                                    dbrootList = dbrootList + ",";
                            }
                        } catch (...) {
                            cout << "ERROR: Problem setting DBRoot ID in the erydb System Configuration file" << endl;
                            exit(1);
                        }
                    }
                    vector <string> dbroots;
                    string tempdbrootList;
                    while (true) {
                        dbroots.clear();
                        bool matchFound = false;

                        prompt = "Enter the list (Nx,Ny,Nz) or range (Nx-Nz) of DBRoot IDs assigned to module '" + newModuleName + "' (" + dbrootList + ") > ";
                        pcommand = callReadline(prompt.c_str());
                        if (pcommand) {
                            if (strlen(pcommand) > 0) {
                                tempdbrootList = pcommand;
                                callFree(pcommand);
                            } else
                                tempdbrootList = dbrootList;
                        }

                        if (tempdbrootList.empty()) {
                            if (noPrompting)
                                exit(1);
                            continue;
                        }

                        //check for range
                        int firstID;
                        int lastID;
                        string::size_type pos = tempdbrootList.find("-", 0);
                        if (pos != string::npos) {
                            firstID = atoi(tempdbrootList.substr(0, pos).c_str());
                            lastID = atoi(tempdbrootList.substr(pos + 1, 200).c_str());

                            if (firstID >= lastID) {
                                cout << "Invalid Entry, please re-enter" << endl;
                                if (noPrompting)
                                    exit(1);
                                continue;
                            } else {
                                for (int id = firstID; id < lastID + 1; id++) {
                                    dbroots.push_back(oam.itoa(id));
                                }
                            }
                        } else {
                            boost::char_separator<char> sep(",");
                            boost::tokenizer< boost::char_separator<char> > tokens(tempdbrootList, sep);
                            for (boost::tokenizer< boost::char_separator<char> >::iterator it = tokens.begin();
                                it != tokens.end();
                                ++it) {
                                dbroots.push_back(*it);
                            }
                        }

                        ////if pm1, make sure DBRoot #1 is in the list
                        //if (newModuleName == "pm1") {
                        //    bool found = false;
                        //    std::vector<std::string>::iterator list = dbroots.begin();
                        //    for (; list != dbroots.end(); list++) {
                        //        if (*list == "1") {
                        //            found = true;
                        //            break;
                        //        }
                        //    }
                        //    if (!found) {
                        //        cout << endl << "Invalid Entry, Module pm1 has to have DBRoot #1 assigned to it, please 	re-enter" << endl;
                        //        if (noPrompting)
                        //            exit(1);
                        //        continue;
                        //    }
                        //}

                        //check and see if DBRoot ID already used
                        std::vector<std::string>::iterator list = dbroots.begin();
                        for (; list != dbroots.end(); list++) {
                            bool inUse = false;
                            matchFound = false;
                            DBRootList::iterator list1 = dbrootlist.begin();
                            for (; list1 != dbrootlist.end(); list1++) {
                                if (*list == (*list1).dbrootID) {
                                    cout << "Invalid Entry, DBRoot ID " + *list + " already assigned to '" + (*list1).moduleName + "', please re-enter" << endl;
                                    if (noPrompting)
                                        exit(1);
                                    inUse = true;
                                    break;
                                }
                            }

                            if (inUse)
                                break;
                            else {	// if upgrade, dont allow a new DBRoot id to be entered
                                if (reuseConfig == "y") {//不允许新增
                                    DBRootConfigList::iterator pt = dbrootConfigList.begin();
                                    for (; pt != dbrootConfigList.end(); pt++) {
                                        if (*list == oam.itoa(*pt)) {
                                            matchFound = true;
                                            break;
                                        }
                                    }
                                    if (!matchFound) {
                                        //get any unassigned DBRoots
                                        DBRootConfigList undbrootlist;
                                        try {
                                            oam.getUnassignedDbroot(undbrootlist);
                                        } catch (...) {}

                                        if (!undbrootlist.empty()) {
                                            DBRootConfigList::iterator pt1 = undbrootlist.begin();
                                            for (; pt1 != undbrootlist.end(); pt1++) {
                                                if (*list == oam.itoa(*pt1)) {
                                                    matchFound = true;
                                                    break;
                                                }
                                            }
                                        }

                                        if (!matchFound) {
                                            cout << "Invalid Entry, DBRoot ID " + *list + " doesn't exist, can't add a new DBRoot during upgrade process, please re-enter" << endl;
                                            if (noPrompting)
                                                exit(1);
                                            break;
                                        }
                                    }
                                } else	// new install, set to found
                                    matchFound = true;
                            }
                        }
                        if (matchFound)
                            break;
                    }

                    int id = 1;
                    std::vector<std::string>::iterator it = dbroots.begin();
                    for (; it != dbroots.end(); it++, ++id) {
                        //save Nic host name and IP
                        dbrootmodule.moduleName = newModuleName;
                        dbrootmodule.dbrootID = *it;
                        dbrootlist.push_back(dbrootmodule);

                        //store DBRoot ID
                        string moduledbrootid = "ModuleDBRootID" + oam.itoa(moduleID) + "-" + oam.itoa(id) + "-" + oam.itoa(i + 1);
                        try {
                            sysConfig->setConfig(ModuleSection, moduledbrootid, *it);
                        } catch (...) {
                            cout << "ERROR: Problem setting DBRoot ID in the erydb System Configuration file" << endl;
                            exit(1);
                        }
                        string DBrootID = "DBRoot" + *it;
                        string pathID = installDir + "/data" + *it;
                        try {
                            sysConfig->setConfig(SystemSection, DBrootID, pathID);
                        } catch (...) {
                            cout << "ERROR: Problem setting DBRoot in the erydb System Configuration file" << endl;
                            return false;
                        }

                        //create data directory
                        cmd = "mkdir " + pathID + " > /dev/null 2>&1";
                        system(cmd.c_str());
                        cmd = "chmod 755 " + pathID + " > /dev/null 2>&1";
                        system(cmd.c_str());
                        //get EC2 volume name and info
                        if (DBRootStorageType == "external" && amazonInstall) {
                            cout << endl << "*** Setup External EBS Volume for DBRoot #" << *it << " ***" << endl << endl;

                            string volumeNameID = "PMVolumeName" + *it;
                            string volumeName = oam::UnassignedName;
                            string deviceNameID = "PMVolumeDeviceName" + *it;
                            string deviceName = oam::UnassignedName;
                            string amazonDeviceNameID = "PMVolumeAmazonDeviceName" + *it;
                            string amazondeviceName = oam::UnassignedName;

                            try {
                                volumeName = sysConfig->getConfig(InstallSection, volumeNameID);
                                deviceName = sysConfig->getConfig(InstallSection, deviceNameID);
                                amazondeviceName = sysConfig->getConfig(InstallSection, amazonDeviceNameID);
                            } catch (...) {
                            }

                            if (reuseConfig == "n" && (volumeName == oam::UnassignedName || volumeName.empty() || volumeName == "")) {
                                string create = "y";

                                cout << "*** NOTE: You have the option to provide an" << endl;
                                cout << "          existing EBS Volume ID or have a Volume created" << endl << endl;

                                while (true) {
                                    pcommand = callReadline("Create a new EBS volume for DBRoot #" + *it + " ?  [y,n] (y) > ");
                                    if (pcommand) {
                                        if (strlen(pcommand) > 0) create = pcommand;
                                        callFree(pcommand);
                                    }
                                    if (create == "y" || create == "n")
                                        break;
                                    else
                                        cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                                    create = "y";
                                    if (noPrompting)
                                        exit(1);
                                }

                                if (create == "n") {
                                    prompt = "Enter Volume ID for '" + DBrootID + "' (" + volumeName + ") > ";
                                    pcommand = callReadline(prompt.c_str());
                                    if (pcommand) {
                                        if (strlen(pcommand) > 0) volumeName = pcommand;
                                        callFree(pcommand);
                                    }

                                    //get device name based on DBRoot ID
                                    oam::storageID_t st;
                                    try {
                                        st = oam.getAWSdeviceName(atoi((*it).c_str()));
                                    } catch (...) {}

                                    deviceName = boost::get<0>(st);
                                    amazondeviceName = boost::get<1>(st);

                                    // update fstabs
                                    string entry = oam.updateFstab(amazondeviceName, *it);
                                } else {
                                    // create amazon ebs DBRoot
                                    try {
                                        DBRootConfigList dbrootlist;
                                        dbrootlist.push_back(atoi((*it).c_str()));

                                        oam.addDbroot(1, dbrootlist, PMVolumeSize);

                                        sleep(2);
                                        try {
                                            volumeName = sysConfig->getConfig(InstallSection, volumeNameID);
                                            deviceName = sysConfig->getConfig(InstallSection, deviceNameID);
                                            amazondeviceName = sysConfig->getConfig(InstallSection, amazonDeviceNameID);
                                        } catch (...) {
                                        }
                                    } catch (exception& e) {
                                        cout << endl << "**** addDbroot Failed: " << e.what() << endl;
                                        exit(1);
                                    }
                                }
                            } else {
                                prompt = "Enter Volume ID for '" + DBrootID + "' (" + volumeName + ") > ";
                                pcommand = callReadline(prompt.c_str());
                                if (pcommand) {
                                    if (strlen(pcommand) > 0) volumeName = pcommand;
                                    callFree(pcommand);
                                }

                                // update fstabs
                                string entry = oam.updateFstab(amazondeviceName, *it);

                            }

                            //write volume and device name
                            try {
                                sysConfig->setConfig(InstallSection, volumeNameID, volumeName);
                                sysConfig->setConfig(InstallSection, deviceNameID, deviceName);
                                sysConfig->setConfig(InstallSection, amazonDeviceNameID, amazondeviceName);
                            } catch (...) {
                                cout << "ERROR: Problem setting Volume/Device Names in the erydb System Configuration file" << endl;
                                return false;
                            }

                            if (!writeConfig(sysConfig)) {
                                cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                                exit(1);
                            }

                            // check volume for attach and try to attach if not
                            if (!attachVolume(newModuleHostName, volumeName, deviceName, pathID)) {
                                cout << "attachVolume error" << endl;
                                exit(1);
                            }
                        }
                    }
                    //store number of dbroots
                    moduledbrootcount = "ModuleDBRootCount" + oam.itoa(moduleID) + "-" + oam.itoa(i + 1);
                    try {
                        sysConfig->setConfig(ModuleSection, moduledbrootcount, oam.itoa(dbroots.size()));
                    } catch (...) {
                        cout << "ERROR: Problem setting DBRoot count in the erydb System Configuration file" << endl;
                        exit(1);
                    }
                    //total dbroots on the system
                    DBRootCount = DBRootCount + dbroots.size();
                }
                if (!writeConfig(sysConfig)) {
                    cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                    exit(1);
                }
                if (moduleType == "pm" && moduleDisableState == oam::ENABLEDSTATE)
                    performancemodulelist.push_back(performancemodule);
            } //end of k (moduleCount) loop

            if (moduleSkip) {
                moduleSkip = false;
                moduleID = saveModuleID;
            } else
                break;
        }

        if (!writeConfig(sysConfig)) {
            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
            exit(1);
        }

    } //end of i for loop

    try {
        sysConfig->setConfig(SystemSection, "DBRootCount", oam.itoa(DBRootCount));
    } catch (...) {
        cout << "ERROR: Problem setting DBRoot Count in the erydb System Configuration file" << endl;
        exit(1);
    }

    //if cloud, copy fstab in module tmp dir
    if (amazonInstall) {
        if (!copyFstab("pm1"))
            cout << "copyFstab error" << endl;
    }

    //check 'files per parition' with number of dbroots
    checkFilesPerPartion(DBRootCount, sysConfig);

    //setup DBRM Controller
    sysConfig->setConfig("DBRM_Controller", "NumWorkers", oam.itoa(DBRMworkernodeID));

    //set ConnectionsPerPrimProc
    try {
        sysConfig->setConfig("PrimitiveServers", "ConnectionsPerPrimProc", oam.itoa(maxPMNicCount * 2));
    } catch (...) {
        cout << "ERROR: Problem setting ConnectionsPerPrimProc in the erydb System Configuration file" << endl;
        exit(1);
    }

    //set the PM Ports based on Number of PM modules equipped, if any equipped
    int minPmPorts = 2;
    sysConfig->setConfig("PrimitiveServers", "Count", oam.itoa(pmNumber));
    if(maxPMNicCount>1){
        //sysConfig->setConfig("PrimitiveServers", "ServerThreads", oam.itoa(maxPMNicCount));
    }
    int pmPorts = pmNumber * (maxPMNicCount * 2);
    if (pmPorts < minPmPorts)
        pmPorts = minPmPorts;

    if (pmNumber > 0) {
        const string PM = "PMS";
        for (int pmsID = 1; pmsID < pmPorts + 1; ) {
            for (unsigned int j = 1; j < maxPMNicCount + 1; j++) {
                PerformanceModuleList::iterator list1 = performancemodulelist.begin();
                for (; list1 != performancemodulelist.end(); list1++) {
                    string pmName = PM + oam.itoa(pmsID);
                    string IpAddr;
                    switch (j) {
                    case 1:
                        IpAddr = (*list1).moduleIP1;
                        break;
                    case 2:
                        IpAddr = (*list1).moduleIP2;
                        break;
                    case 3:
                        IpAddr = (*list1).moduleIP3;
                        break;
                    case 4:
                        IpAddr = (*list1).moduleIP4;
                        break;
                    }
                    if (!IpAddr.empty() && IpAddr != oam::UnassignedIpAddr) {
                        sysConfig->setConfig(pmName, "IPAddr", IpAddr);
                        if(maxPMNicCount>1){
                            //sysConfig->setConfig(pmName, "ListenAddr", IpAddr);
                        }
                        sysConfig->setConfig(pmName, "Port", "8620");
                        pmsID++;
                        if (pmsID > pmPorts)
                            break;
                    }
                }
                if (pmsID > pmPorts)
                    break;
            }
        }
    }

    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        exit(1);
    }

    //setup local OS Files
    if (!setOSFiles(parentOAMModuleName, DBRootStorageType)) {
        cout << "setOSFiles error" << endl;
        exit(1);
    }
    //check if dbrm data resides in older directory path and inform user if it does
    dbrmDirCheck();
    //create directories on dbroot1
    if (!createMetaDataDir(DBRootStorageType)) {
        cout << "createDbrootDirs error" << endl;
        exit(1);
    }
    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        exit(1);
    }
    if (IserverTypeInstall == oam::INSTALL_COMBINE_DM_UM_PM && pmNumber == 1) {
        //run the mysql / mysqld setup scripts
        cout << endl << "===== Running the erydb setup scripts =====" << endl << endl;
        checkMysqlPort(mysqlPort, sysConfig);
        // call the mysql setup scripts
        mysqlSetup();
        sleep(5);
    }
    int thread_id = 0;
    pthread_t thr[childmodulelist.size()];
    /* create a thread_data_t argument array */
    thread_data_t thr_data[childmodulelist.size()];
    string install = "y";
    //remote install
    if (IserverTypeInstall != oam::INSTALL_COMBINE_DM_UM_PM || pmNumber > 1) {
        // perform remote install of other servers in the system
        cout << endl << "===== System Installation =====" << endl << endl;
        cout << "System Configuration is complete, System Installation is the next step." << endl;
        while (true) {
            pcommand = callReadline("Would you like to continue with the System Installation? [y,n] (y) > ");
            if (pcommand) {
                if (strlen(pcommand) > 0) install = pcommand;
                callFree(pcommand);
            }
            if (install == "y" || install == "n")
                break;
            else
                cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
            install = "y";
            if (noPrompting)
                exit(1);
        }

        if (install == "y") {
            SystemSoftware systemsoftware;
            try {
                oam.getSystemSoftware(systemsoftware);
            } catch (exception& e) {
                cout << " ERROR: reading getSystemSoftware API" << endl;
                exit(1);
            }
            cout << endl;
            if (EEPackageType == "rpm") {
                cout << "Performing an erydb System install using RPM packages" << endl;
                cout << "located in the " + HOME + " directory." << endl << endl;
            } else {
                if (EEPackageType == "binary") {
                    cout << "Performing an erydb System install using a Binary package" << endl;
                    cout << "located in the " + HOME + " directory." << endl << endl;
                } else {
                    cout << "Performing an erydb System install using using DEB packages" << endl;
                    cout << "located in the " + HOME + " directory." << endl;
                }
            }
            //check if pkgs are located in $HOME directory
            string version = systemsoftware.Version + "-" + systemsoftware.Release;
            if (EEPackageType == "rpm"){
                erydbPackage = HOME + "/" + "erydb-" + version + "*.rpm.tar.gz";
            } else{
                if (EEPackageType == "deb")
                    erydbPackage = HOME + "/" + "erydb-" + version + "*.deb.tar.gz";
                else
                    erydbPackage = HOME + "/" + "erydb-" + version + "*.bin.tar.gz";
            }
            if (!pkgCheck(erydbPackage))
                exit(1);
            if (password.empty()) {
                cout << endl;
                cout << "Next step is to enter the password to access the other Servers." << endl;
                cout << "This is either your password or you can default to using a ssh key" << endl;
                cout << "If using a password, the password needs to be the same on all Servers." << endl << endl;
            }
            while (true) {
                char  *pass1, *pass2;
                if (noPrompting) {
                    cout << "Enter password, hit 'enter' to default to using a ssh key, or 'exit' > " << endl;
                    if (password.empty())
                        password = "ssh";
                    break;
                }
                //check for command line option password
                if (!password.empty())
                    break;
                pass1 = getpass("Enter password, hit 'enter' to default to using a ssh key, or 'exit' > ");
                if (strcmp(pass1, "") == 0) {
                    password = "ssh";
                    break;
                }
                if (pass1 == "exit")
                    exit(0);
                string p1 = pass1;
                pass2 = getpass("Confirm password > ");
                string p2 = pass2;
                if (p1 == p2) {
                    password = p2;
                    break;
                } else {
                    cout << "Password mismatch, please re-enter" << endl;
                }
            }
            //add single quote for special characters
            if (password != "ssh") {
                password = "'" + password + "'";
            }
            checkSystemMySQLPort(mysqlPort, sysConfig, USER, password, childmodulelist, IserverTypeInstall, pmwithum);
            if ((IserverTypeInstall == oam::INSTALL_COMBINE_DM_UM_PM) ||
                ((IserverTypeInstall != oam::INSTALL_COMBINE_DM_UM_PM) && pmwithum)) {
                cout << endl << "===== Running the erydb setup scripts =====" << endl << endl;
                // call the mysql setup scripts
                mysqlSetup();
                sleep(5);
            }
            if (mysqlpw != oam::UnassignedName || mysqlpw != " ") {
                mysqlpw = "'" + mysqlpw + "'";
                pwprompt = "--password=" + mysqlpw;
            }
            ChildModuleList::iterator list1 = childmodulelist.begin();
            for (; list1 != childmodulelist.end(); list1++) {
                string remoteModuleName = (*list1).moduleName;
                string remoteModuleIP = (*list1).moduleIP;
                string remoteHostName = (*list1).hostName;
                string remoteModuleType = remoteModuleName.substr(0, MAX_MODULE_TYPE_SIZE);
                string debug_logfile;
                string logfile;
                if (remote_installer_debug == "1") {
                    logfile = "/tmp/";
                    logfile += remoteModuleName + "_" + EEPackageType + "_install.log";
                    debug_logfile = " > " + logfile;
                }
                if (remoteModuleType == "um" ||
                    (remoteModuleType == "pm" && IserverTypeInstall == oam::INSTALL_COMBINE_DM_UM_PM) ||
                    (remoteModuleType == "pm" && pmwithum)) {
                    cout << endl << "----- Performing Install on '" + remoteModuleName + " / " + remoteHostName + "' -----" << endl << endl;
                    if (remote_installer_debug == "1") {
                        cout << "Install log file is located here: " + logfile << endl << endl;
                    }
                    if (EEPackageType != "binary") {
                        string temppwprompt = pwprompt;
                        if (pwprompt == " ")
                            temppwprompt = "none";
                        //run remote installer script
                        cmd = installDir + "/bin/user_installer.sh " + remoteModuleName + " " + remoteModuleIP + " " + password + " " + version + 
                            " initial " + EEPackageType + " " + nodeps + " " + temppwprompt + " " + mysqlPort + " " + remote_installer_debug + " " + debug_logfile;
                        //cout << cmd << endl;
                        if (remote_installer_debug == "1") {
                            cout<< remoteModuleName<<" install: "<< cmd <<endl;
                        }
                        if (thread_remote_installer) {
                            thr_data[thread_id].command = cmd;
                            int status = pthread_create(&thr[thread_id], NULL, (void*(*)(void*)) &remoteInstallThread, &thr_data[thread_id]);
                            if (status != 0) {
                                cout << "remoteInstallThread failed for " << remoteModuleName << ", exiting" << endl;
                                exit(1);
                            }
                            thread_id++;
                        } else {
                            int rtnCode = system(cmd.c_str());
                            if (WEXITSTATUS(rtnCode) != 0) {
                                cout << endl << "Error returned from user_installer.sh" << endl;
                                exit(1);
                            }
                            //check for mysql password on remote UM
                            if (pwprompt == " ") {
                                //start mysqld
                                cmd = installDir + "/bin/remote_command.sh " + remoteModuleIP + " " + password + " '" + installDir + "/mysql/mysql-erydb start'";
                                int rtnCode = system(cmd.c_str());
                                if (WEXITSTATUS(rtnCode) != 0) {
                                    cout << endl << "Error returned from mysql-erydb start" << endl;
                                    exit(1);
                                }
                                //try to login
                                cmd = installDir + "/bin/remote_command.sh " + remoteModuleIP + " " + password + " '" + installDir + "/mysql/bin/mysql --defaults-file=" + installDir + "/mysql/my.cnf -u root " + pwprompt + " -e status' 1 > /tmp/erydbmysql.log 2>&1";
                                rtnCode = system(cmd.c_str());
                                if (WEXITSTATUS(rtnCode) != 0) {
                                    cout << endl << "Error returned from remote_command.sh" << endl;
                                    exit(1);
                                }
                                if (oam.checkLogStatus("/tmp/erydbmysql.log", "ERROR .my.cnf")) {
                                    // password needed check and get password from remote UM
                                    cmd = installDir + "/bin/remote_command.sh " + remoteModuleIP + " " + password + " '" + installDir + "bin/getMySQLpw > /tmp/mysqlpw.log 2>&1";
                                    rtnCode = system(cmd.c_str());
                                    if (WEXITSTATUS(rtnCode) != 0) {
                                        cout << endl << "erydb login failure, MySQL Root password is set." << endl;
                                        cout << "Need erydb password configuration file " + HOME + "/.my.cnf on " << remoteModuleName << endl;
                                        exit(1);
                                    }
                                    //get password from local tmp file
                                    mysqlpw = getmysqlpw("/tmp/mysqlpw.log");
                                    if (mysqlpw != oam::UnassignedName) {
                                        mysqlpw = "'" + mysqlpw + "'";
                                        pwprompt = "--password=" + mysqlpw;
                                    }
                                    cmd = installDir + "/bin/remote_command.sh " + remoteModuleIP + " " + password + " '" + installDir + "/mysql/bin/mysql --defaults-file=" + installDir + "/mysql/my.cnf -u root " + pwprompt + " -e status' 1 > /tmp/erydbmysql.log 2>&1";
                                    rtnCode = system(cmd.c_str());
                                    if (WEXITSTATUS(rtnCode) != 0) {
                                        cout << endl << "erydb  login failure, password mismatch in " + HOME + ".my.cnf on " << remoteModuleName << endl;
                                        exit(1);
                                    }
                                } else {
                                    if (!oam.checkLogStatus("/tmp/erydbmysql.log", "EryDB")) {
                                        cout << endl << "ERROR: erydb runtime error, exit..." << endl << endl;
                                        system("cat /tmp/erydbmysql.log");
                                        exit(1);
                                    } else {
                                        cout << endl << "Additional erydb Installation steps Successfully Completed on '" + remoteModuleName + "'" << endl << endl;
                                        cmd = installDir + "/bin/remote_command.sh " + remoteModuleIP + " " + password + " '" + installDir + "/mysql/mysql-erydb stop'";
                                        int rtnCode = system(cmd.c_str());
                                        if (WEXITSTATUS(rtnCode) != 0) {
                                            cout << endl << "Error returned from mysql-erydb stop" << endl;
                                            exit(1);
                                        }
                                        unlink("/tmp/erydbmysql.log");
                                        break;
                                    }
                                }
                                //re-run post-mysql-install with password
                                cmd = installDir + "/bin/remote_command.sh " + remoteModuleIP + " " + password + " '" + installDir + "/bin/post-mysql-install " + pwprompt + "' < /tmp/post-mysql-install.log";
                                rtnCode = system(cmd.c_str());
                                if (WEXITSTATUS(rtnCode) != 0) {
                                    cout << endl << "Error returned from post-mysql-install, check /tmp/post-mysql-install.log" << endl;
                                    exit(1);
                                } else
                                    cout << endl << "post-mysql-install Successfully Completed" << endl;
                            }
                        }
                    } else {	// do a binary package install
                        string binservertype = serverTypeInstall;
                        if (pmwithum) {
                            binservertype = "pmwithum";
                        }
                        //check my.cnf port in-user on remote node
//						checkRemoteMysqlPort(remoteModuleIP, remoteModuleName, USER, password, mysqlPort, sysConfig);
                        cmd = installDir + "/bin/binary_installer.sh " + remoteModuleName + " " + remoteModuleIP + " " + password + " " + erydbPackage + " " + remoteModuleType +
                            " initial " + binservertype + " " + mysqlPort + " " + remote_installer_debug + " " + installDir +" " + debug_logfile;
                        if (remote_installer_debug == "1") {
                            cout << remoteModuleName << " install: " << cmd << endl;
                        }
                        if (thread_remote_installer) {
                            thr_data[thread_id].command = cmd;
                            int status = pthread_create(&thr[thread_id], NULL, (void*(*)(void*)) &remoteInstallThread, &thr_data[thread_id]);
                            if (status != 0) {
                                cout << "remoteInstallThread failed for " << remoteModuleName << ", exiting" << endl;
                                exit(1);
                            }
                            thread_id++;
                        } else {
                            int rtnCode = system(cmd.c_str());
                            if (WEXITSTATUS(rtnCode) != 0) {
                                cout << endl << "Error returned from user_installer.sh" << endl;
                                exit(1);
                            }
                        }
                    }
                } else {
                    if ((remoteModuleType == "pm" && IserverTypeInstall != oam::INSTALL_COMBINE_DM_UM_PM) /*||
                        (remoteModuleType == "pm" && !pmwithum)*/) {
                        cout << endl << "----- Performing Install on '" + remoteModuleName + " / " + remoteHostName + "' -----" << endl << endl;
                        if (remote_installer_debug == "1") {
                            cout << "Install log file is located here: " + logfile << endl << endl;
                        }
                        if (EEPackageType != "binary") {
                            //run remote installer script
                            cmd = installDir + "/bin/performance_installer.sh " + remoteModuleName + " " + remoteModuleIP + " " + password + " " + version + " initial " + 
                                EEPackageType + " " + nodeps + " " + remote_installer_debug + " " + debug_logfile;
                            //cout << cmd << endl;
                            if (remote_installer_debug == "1") {
                                cout << remoteModuleName << " install: " << cmd << endl;
                            }
                            if (thread_remote_installer) {
                                thr_data[thread_id].command = cmd;
                                int status = pthread_create(&thr[thread_id], NULL, (void*(*)(void*)) &remoteInstallThread, &thr_data[thread_id]);
                                if (status != 0) {
                                    cout << "remoteInstallThread failed for " << remoteModuleName << ", exiting" << endl;
                                    exit(1);
                                }
                                thread_id++;
                            } else {
                                int rtnCode = system(cmd.c_str());
                                if (WEXITSTATUS(rtnCode) != 0) {
                                    cout << endl << "Error returned from performance_installer.sh" << endl;
                                    exit(1);
                                }
                            }
                        } else {	// do a binary package install
                            string binservertype = serverTypeInstall;
                            if (pmwithum)
                                binservertype = "pmwithum";
                            cmd = installDir + "/bin/binary_installer.sh " + remoteModuleName + " " + remoteModuleIP +
                                " " + password + " " + erydbPackage + " " + remoteModuleType + " initial " +
                                binservertype + " " + mysqlPort + " " + remote_installer_debug + " " + installDir + " " +
                                debug_logfile;
                            if (remote_installer_debug == "1") {
                                cout << remoteModuleName << " install: " << cmd << endl;
                            }
                            if (thread_remote_installer) {
                                thr_data[thread_id].command = cmd;
                                int status = pthread_create(&thr[thread_id], NULL, (void*(*)(void*)) &remoteInstallThread, &thr_data[thread_id]);
                                if (status != 0) {
                                    cout << "remoteInstallThread failed for " << remoteModuleName << ", exiting" << endl;
                                    exit(1);
                                }
                                thread_id++;
                            } else {
                                int rtnCode = system(cmd.c_str());
                                if (WEXITSTATUS(rtnCode) != 0) {
                                    cout << endl << "Error returned from user_installer.sh" << endl;
                                    exit(1);
                                }
                            }
                        }
                    }
                }
            }
            if (thread_remote_installer) {
                //wait until remove install Thread Count is at zero or hit timeout
                cout << endl << "erydb Package being installed, please wait ...";
                cout.flush();
                /* block until all threads complete */
                for (thread_id = 0; thread_id < (int)childmodulelist.size(); ++thread_id) {
                    pthread_join(thr[thread_id], NULL);
                }
                cout << "  DONE" << endl;
            }
        }
    }

    //configure data redundancy
    string glusterconfig = installDir + "/bin/glusterconf";
    if (gluster) {
        cout << endl;
        string start = "y";
        if (reuseConfig == "y")
            start = "n";
        while (true) {
            pcommand = callReadline("Would you like to configure erydb GlusterFS Data Redundancy? [y,n] (" + start + ") > ");
            if (pcommand) {
                if (strlen(pcommand) > 0) start = pcommand;
                callFree(pcommand);
            }
            if (start == "y" || start == "n")
                break;
            else
                cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
            start = "y";
            if (noPrompting)
                exit(1);
        }
        if (start == "y") {
            cout << endl << "===== Configuring erydb Data Redundancy Functionality =====" << endl << endl;
            int ret = system(glusterconfig.c_str());
            if (WEXITSTATUS(ret) != 0) {
                cerr << endl << "There was an error in the Data Redundancy setup, exiting..." << endl;
                exit(1);
            }
        }
    }
    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        exit(1);
    }
    //check if local erydb system logging is working
    cout << endl << "===== Checking erydb System Logging Functionality =====" << endl << endl;
    if (rootUser) {
        cmd = installDir + "/bin/syslogSetup.sh status  > /dev/null 2>&1";
    } else {
        cmd = "sudo " + installDir + "/bin/syslogSetup.sh --installdir=" + installDir + " status  > /dev/null 2>&1";
    }
    int ret = system(cmd.c_str());
    if (WEXITSTATUS(ret) != 0) {
        cerr << "WARNING: The erydb system logging not correctly setup and working" << endl;
    } else {
        cout << "The erydb system logging is setup and working on local server" << endl;
    }
    cout << endl << "erydb System Configuration and Installation is Completed" << endl;
    // startup erydb
    if (IserverTypeInstall != oam::INSTALL_COMBINE_DM_UM_PM || pmNumber > 1) {
        // perform erydb system startup
        cout << endl << "===== erydb System Startup =====" << endl << endl;
        string start = "y";
        cout << "System Installation is complete. If any part of the install failed," << endl;
        cout << "the problem should be investigated and resolved before continuing." << endl << endl;
        while (true) {
            pcommand = callReadline("Would you like to startup the erydb System? [y,n] (y) > ");
            if (pcommand) {
                if (strlen(pcommand) > 0) start = pcommand;
                callFree(pcommand);
            }
            if (start == "y" || start == "n")
                break;
            else
                cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
            start = "y";
            if (noPrompting)
                exit(1);
        }
        //start cluster
        if (start == "y") {
            if (hdfs) {
                cout << endl << "----- Starting erydb Service on all Modules -----" << endl << endl;
                string cmd = "pdsh -a '" + installDir + "/bin/erydb restart' > /tmp/postConfigure.pdsh 2>&1";
                system(cmd.c_str());
                if (oam.checkLogStatus("/tmp/postConfigure.pdsh", "exit")) {
                    cout << endl << "ERROR: Starting erydb Service failue, check /tmp/postConfigure.pdsh. exit..." << endl;
                    exit(1);
                }
            } else {
                if (password.empty()) {
                    while (true) {
                        char  *pass1, *pass2;
                        if (noPrompting) {
                            cout << "Enter your password, hit 'enter' to default to using a ssh key, or 'exit' > " << endl;
                            if (password.empty())
                                password = "ssh";
                            break;
                        }
                        //check for command line option password
                        if (!password.empty())
                            break;
                        pass1 = getpass("Enter your password, hit 'enter' to default to using a ssh key, or 'exit' > ");
                        if (strcmp(pass1, "") == 0) {
                            password = "ssh";
                            break;
                        }
                        if (strcmp(pass1, "exit") == 0)
                            exit(0);
                        string p1 = pass1;
                        pass2 = getpass("Confirm password > ");
                        string p2 = pass2;
                        if (p1 == p2) {
                            password = p2;
                            break;
                        } else
                            cout << "Password mismatch, please re-enter" << endl;
                    }
                    //add single quote for special characters
                    if (password != "ssh") {
                        password = "'" + password + "'";
                    }
                }
                ChildModuleList::iterator list1 = childmodulelist.begin();
                for (; list1 != childmodulelist.end(); list1++) {
                    string remoteModuleName = (*list1).moduleName;
                    string remoteModuleIP = (*list1).moduleIP;
                    string remoteHostName = (*list1).hostName;
                    //run remote command script
                    cout << endl << "----- Starting erydb on '" + remoteModuleName + "' -----" << endl << endl;
                    if (install == "n") {	// didnt do a full install, push the config file
                        cmd = installDir + "/bin/remote_scp_put.sh " + remoteModuleIP + " " + installDir + "/etc/erydb.xml  > /dev/null 2>&1";
                        system(cmd.c_str());
                    }
                    cmd = installDir + "/bin/remote_command.sh " + remoteModuleIP + " " + password + " '" + installDir + "/bin/erydb restart' 0";
                    int rtnCode = system(cmd.c_str());
                    if (WEXITSTATUS(rtnCode) != 0) {
                        cout << "Error with running remote_command.sh" << endl;
                    } else {
                        cout << "erydb successfully started" << endl;
                    }
                }
                //start erydb on parentOamModule server
                cout << endl << "----- Starting erydb on "<< parentOAMModuleHostName <<" server -----" << endl << endl;
                if (parentOAMModuleHostName == localHostName) {
                    cmd = installDir + "/bin/erydb restart > /dev/null 2>&1";
                } else { 
                    cmd = installDir + "/bin/remote_command.sh " + parentOAMModuleIPAddr + " '" + password + "'  '"+ installDir + "/bin/erydb restart' 0 ";
                }
                int rtnCode = system(cmd.c_str());
                if (WEXITSTATUS(rtnCode) != 0) {
                    cout << "Error Starting erydb local module" << endl;
                    cout << "Installation Failed, exiting" << endl;
                    exit(1);
                } else {
                    cout << "erydb successfully started" << endl;
                }
            }
        } else {
            cout << endl << "You choose not to Start the erydb Software at this time." << endl;
            exit(1);
        }
    } else {// Single Server start
        cout << endl << "===== erydb System Startup =====" << endl << endl;
        string start = "y";
        cout << "System Installation is complete." << endl;
        cout << "If an error occurred while running the erydb setup scripts," << endl;
        cout << "this will need to be corrected and postConfigure will need to be re-run." << endl << endl;
        while (true) {
            pcommand = callReadline("Would you like to startup the erydb System? [y,n] (y) > ");
            if (pcommand) {
                if (strlen(pcommand) > 0) start = pcommand;
                callFree(pcommand);
            }
            if (start == "y" || start == "n")
                break;
            else
                cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
            start = "y";
            if (noPrompting)
                exit(1);
        }
        if (start == "y") {
            //start erydb on local server
            cout << endl << "----- Starting erydb on local Server '" + parentOAMModuleName + "' -----" << endl << endl;
            string cmd = installDir + "/bin/erydb restart > /dev/null 2>&1";
            int rtnCode = system(cmd.c_str());
            if (WEXITSTATUS(rtnCode) != 0) {
                cout << "Error Starting erydb local module" << endl;
                cout << "Installation Failed, exiting" << endl;
                exit(1);
            } else
                cout << endl << "erydb successfully started" << endl;
        } else {
            cout << endl << "You choose not to Start the erydb Software at this time." << endl;
            exit(1);
        }
    }
    cout << endl << "erydb Database Platform Starting, please wait .";
    cout.flush();
    if (waitForActive()) {
        cout << " DONE" << endl;
        if (hdfs) {
            cmd = "bash -c '. " + installDir + "/bin/" + DataFileEnvFile + ";" + installDir + "/bin/dbbuilder 7 > /tmp/dbbuilder.log'";
        } else {
            if (parentOAMModuleHostName == localHostName) {
                cmd = installDir + "/bin/dbbuilder 7 > /tmp/dbbuilder.log";
            } else {
                 cmd =  "ssh " + parentOAMModuleIPAddr + " \"" + installDir + "/bin/dbbuilder 7\"  > /tmp/dbbuilder.log";
            }
        }
        system(cmd.c_str());
        if (oam.checkLogStatus("/tmp/dbbuilder.log", "System Catalog created")) {
            cout << endl << "System Catalog Successfully Created" << endl;
        } else {
            if (oam.checkLogStatus("/tmp/dbbuilder.log", "System catalog appears to exist")) {
                //cout << endl << "Run MySQL Upgrade.. ";
                //cout.flush();
                //send message to procmon's to run upgrade script
                //    int status = sendUpgradeRequest(IserverTypeInstall, pmwithum);
                //if (status != 0) {
                //    cout << endl << "erydb Install Failed" << endl << endl;
                //    exit(1);
                //} else
                //    cout << " DONE" << endl;
            } else {
                cout << endl << "System Catalog Create Failure" << endl;
                cout << "Check latest log file in /tmp/dbbuilder.log.*" << endl;
                exit(1);
            }
        }
        //set mysql replication, if wasn't setup before on system
        if ((mysqlRep && pmwithum) ||
            (mysqlRep && (umNumber > 1)) ||
            (mysqlRep && (pmNumber > 1) && (IserverTypeInstall == oam::INSTALL_COMBINE_DM_UM_PM))) {
            cout << endl << "Run erydb Replication Setup.. ";
            cout.flush();
            //send message to procmon's to run upgrade script
            int status = sendReplicationRequest(IserverTypeInstall, password, mysqlPort, pmwithum);
            if (status != 0) {
                cout << endl << " erydb Install Failed" << endl << endl;
                exit(1);
            } else
                cout << " DONE" << endl;
        }
        cout << endl << "erydb Install Successfully Completed, System is Active" << endl << endl;
        cout << "Enter the following command to define erydb Alias Commands" << endl << endl;
        cout << ". " + installDir + "/bin/erydbAlias" << endl << endl;
        cout << "Enter 'erymysql' to access the erydb SQL console" << endl;
        cout << "Enter 'eryadmin' to access the erydb Admin console" << endl << endl;
    } else {
        cout << " FAILED" << endl;
        cout << endl << "erydb System failed to start, check log files in /var/log/erydb" << endl;
        exit(1);
    }
    exit(0);
}

void setSingleServerInstall(bool startOfflinePrompt) {
    singleServerInstall = "1";
    cout << endl << "Performing the Single Server Install." << endl;
    string cmd = "";
    if (reuseConfig == "n") {
        //setup to use the single server erydb.xml file
        // we know that our Config instance just timestamped itself in the getConfig
        // call above.  if postConfigure is running non-interactively we may get here
        // within the same second which means the changes that are about to happen
        // when erydb.xml gets overwritten will be ignored because of the Config
        // instance won't know to reload
        sleep(2);
        cmd = "rm -f " + installDir + "/etc/erydb.xml.installSave  > /dev/null 2>&1";
        system(cmd.c_str());
        cmd = "mv -f " + installDir + "/etc/erydb.xml " + installDir + "/etc/erydb.xml.installSave  > /dev/null 2>&1";
        system(cmd.c_str());
        cmd = "/bin/cp -f " + installDir + "/etc/erydb.xml.singleserver " + installDir + "/etc/erydb.xml  > /dev/null 2>&1";
        system(cmd.c_str());
    }

    setSystemName();
    cout << endl; 
 
    // setup storage
    if (!storageSetup(false)) {
        cout << "ERROR: Problem setting up storage" << endl;
        exit(1);
    }

    if (hdfs || !rootUser) {
        if (!updateBash())
            cout << "updateBash error" << endl;
    }
    // setup storage
    if (!singleServerDBrootSetup()) {
        cout << "ERROR: Problem setting up DBRoot IDs" << endl;
        exit(1);
    }

    //set system DBRoot count and check 'files per parition' with number of dbroots
    try {
        Oam oam;
        sysConfig->setConfig(SystemSection, "DBRootCount", oam.itoa(DBRootCount));
    } catch (...) {
        cout << "ERROR: Problem setting DBRoot Count in the erydb System Configuration file" << endl;
        exit(1);
    }

    checkFilesPerPartion(DBRootCount, sysConfig);

    //check if dbrm data resides in older directory path and inform user if it does
    dbrmDirCheck();

    if (startOfflinePrompt)
        offLineAppCheck();

    checkMysqlPort(mysqlPort, sysConfig);

    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        exit(1);
    }

    cout << endl << "===== Performing Configuration Setup and erydb Startup =====" << endl;

    cmd = installDir + "/bin/installer dummy.rpm dummy.rpm dummy.rpm dummy.rpm dummy.rpm initial dummy " + reuseConfig + " --nodeps ' ' 1 " + installDir;
    system(cmd.c_str());
    exit(0);
}
/*
 * Check for reuse of RPM saved erydb.xml
 */
bool checkSaveConfigFile() {
    string rpmFileName = installDir + "/etc/erydb.xml";
    string newFileName = installDir + "/etc/erydb.xml.new";

    string extentMapCheckOnly = " ";

    //check if erydb.xml.rpmsave exist
    ifstream File(oldFileName.c_str());
    if (!File) {
        if (noPrompting) {
            cout << endl << "Old Config File not found '" + oldFileName + "', exiting" << endl;
            exit(1);
        }
        return true;
    }
    File.close();

    // If 'oldFileName' isn't configured, exit
    Config* oldSysConfig = Config::makeConfig(oldFileName);

    string oldpm1 = oldSysConfig->getConfig("SystemModuleConfig", "ModuleIPAddr1-1-3");
    if (oldpm1 == "0.0.0.0") {
        if (noPrompting) {
            cout << endl << "Old Config File not Configured, PM1 IP Address entry is '0.0.0.0', '" + oldFileName + "', exiting" << endl;
            exit(1);
        } else
            return true;
    }

    // get single-server system install type
    string temp;
    try {
        temp = oldSysConfig->getConfig(InstallSection, "SingleServerInstall");
    } catch (...) {
    }

    if (!temp.empty())
        singleServerInstall = temp;

    if (singleServerInstall == "y")
        singleServerInstall = "1";
    else
        singleServerInstall = "2";

    if (!noPrompting) {
        cout << endl << "A copy of the erydb Configuration file has been saved during Package install." << endl;
        if (singleServerInstall == "1")
            cout << "It's Configured for a Single Server Install." << endl;
        else
            cout << "It's Configured for a Multi-Server Install." << endl;

        cout << "You have an option of utilizing the configuration data from that file or starting" << endl;
        cout << "with the erydb Configuration File that comes with the erydb Package." << endl;
        cout << "You will only want to utilize the old configuration data when performing the same" << endl;
        cout << "type of install, i.e. Single or Multi-Server" << endl;
    } else {
        cout << "The erydb Configuration Data is taken from " << oldFileName << endl;
    }

    cout << endl;
    while (true) {
        pcommand = callReadline("Do you want to utilize the configuration data from the saved copy? [y,n]  > ");
        if (pcommand) {
            if (strlen(pcommand) > 0) {
                reuseConfig = pcommand;
            } else {
                if (noPrompting)
                    reuseConfig = "y";
                else {
                    cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
                    if (noPrompting)
                        exit(1);
                    continue;
                }
            }

            callFree(pcommand);
        }

        string cmd;
        if (reuseConfig == "y") {
            if (singleServerInstall == "1") {
                cmd = "rm -f " + installDir + "/etc/erydb.xml.installSave  > /dev/null 2>&1";
                system(cmd.c_str());
                cmd = "mv -f " + installDir + "/etc/erydb.xml " + installDir + "/etc/erydb.xml.installSave  > /dev/null 2>&1";
                system(cmd.c_str());
                cmd = "/bin/cp -f " + installDir + "/etc/erydb.xml.singleserver " + installDir + "/etc/erydb.xml  > /dev/null 2>&1";
                system(cmd.c_str());
            }
            break;
        }

        if (reuseConfig == "n") {
            extentMapCheckOnly = "-e";
            break;
        } else
            cout << "Invalid Entry, please enter 'y' for yes or 'n' for no" << endl;
    }

    // clear this entry out to validate updates being made
    Config* sysConfig = Config::makeConfig();
    sysConfig->setConfig("ExeMgr1", "IPAddr", "0.0.0.0");

    for (int retry = 0; retry < 5; retry++) {
        string cmd = "mv -f " + rpmFileName + " " + newFileName;
        int rtnCode = system(cmd.c_str());
        if (WEXITSTATUS(rtnCode) != 0) {
            cout << "Error moving installed version of erydb.xml" << endl;
            return false;
        }

        cmd = "cp " + oldFileName + " " + rpmFileName;
        rtnCode = system(cmd.c_str());
        if (WEXITSTATUS(rtnCode) != 0) {
            cout << "Error moving pkgsave file" << endl;
            return false;
        }

        cmd = "cd " + installDir + "/etc/;../bin/autoConfigure " + extentMapCheckOnly;
        rtnCode = system(cmd.c_str());
        if (WEXITSTATUS(rtnCode) != 0) {
            cout << "Error running autoConfigure" << endl;
            return false;
        }

        cmd = "mv -f " + newFileName + " " + rpmFileName;
        rtnCode = system(cmd.c_str());
        if (WEXITSTATUS(rtnCode) != 0) {
            cout << "Error moving pkgsave file" << endl;
            return false;
        }

        //check to see if updates were made	
        if (sysConfig->getConfig("ExeMgr1", "IPAddr") != "0.0.0.0") {
            //erydb.xml is ready to go, get feature options

            if (MySQLRep == "n") {
                try {
                    MySQLRep = sysConfig->getConfig(InstallSection, "MySQLRep");
                } catch (...) {
                }

                if (MySQLRep == "y")
                    mysqlRep = true;
            }

            if (PMwithUM == "n") {
                //get local query / PMwithUM feature flag
                try {
                    PMwithUM = sysConfig->getConfig(InstallSection, "PMwithUM");
                } catch (...) {
                }

                if (PMwithUM == "y") {
                    pmwithum = true;
                }
            }
            return true;
        }

        sleep(1);
    }

    if (reuseConfig == "n")
        return true;

    cout << "ERROR: Failed to copy data to erydb.xml" << endl;
    return false;

}

/*
 * Setup OS Files by appending the erydb versions
 */

 // /etc OS Files to be updated
string files[] = {
    "rc.local",
    " "
};

bool setOSFiles(string parentOAMModuleName, string DBRootStorageType) {
   string cmd = "scp " + installDir + "/local/etc/ "+ parentOAMModuleHostName +":" + installDir + "/local/ > /dev/null 2>&1";
    system(cmd.c_str());
    if (DBRootStorageType != "external") {
        return true;
    }
    bool remote = false;
    if (parentOAMModuleHostName != localHostName) {
        remote = true;
    }
    string localHostName;
    string password;
     
    bool allfound = true;
    //update /etc files
    for (int i = 0;; ++i) {
        if (files[i] == " ")
            //end of list
            break;
        //create or update date on file to make sure on exist
        if (files[i] == "rc.local") {
            if (remote) {
                string cmd = "ssh " + parentOAMModuleHostName + " touch " + installDir + "/local/etc/" + parentOAMModuleName + "/rc.local.erydb > /dev/null 2>&1";
                if (!rootUser) {
                    cmd = "ssh " + parentOAMModuleHostName + " sudo touch " + installDir + "/local/etc/" + parentOAMModuleName + "/rc.local.erydb > /dev/null 2>&1";
                }
                system(cmd.c_str());
            } else {
                string cmd = "touch " + installDir + "/local/etc/" + parentOAMModuleName + "/rc.local.erydb > /dev/null 2>&1";
                if (!rootUser)
                    cmd = "sudo touch " + installDir + "/local/etc/" + parentOAMModuleName + "/rc.local.erydb > /dev/null 2>&1";
                system(cmd.c_str());
            }
        }
        string fileName = "/etc/" + files[i];
        //make a backup copy before changing
        string cmd;
        if (remote) {
            cmd = "ssh " + parentOAMModuleHostName + " rm -f " + fileName + ".erydbSave";
            if (!rootUser)
                cmd = "ssh " + parentOAMModuleHostName + " sudo rm -f " + fileName + ".erydbSave";
            system(cmd.c_str());
            cmd = "ssh " + parentOAMModuleHostName + " scp " + fileName + " " + fileName + ".erydbSave > /dev/null 2>&1";
            if (!rootUser)
                cmd = "ssh " + parentOAMModuleHostName + " sudo scp " + fileName + " " + fileName + ".erydbSave > /dev/null 2>&1";
            system(cmd.c_str());
            if (rootUser)
                cmd = "ssh " + parentOAMModuleHostName + " \"cat " + installDir + "/local/etc/" + parentOAMModuleName + "/" + files[i] + ".erydb >> " + fileName + "\"";
            else
                cmd = "ssh " + parentOAMModuleHostName + " sudo \"cat " + installDir + "/local/etc/" + parentOAMModuleName + "/" + files[i] + ".erydb >> " + fileName + "\"";
        } else {
            cmd = "rm -f " + fileName + ".erydbSave";
            if (!rootUser)
                cmd = "sudo rm -f " + fileName + ".erydbSave";
            system(cmd.c_str());
            cmd = "cp " + fileName + " " + fileName + ".erydbSave > /dev/null 2>&1";
            if (!rootUser)
                cmd = "sudo cp " + fileName + " " + fileName + ".erydbSave > /dev/null 2>&1";
            system(cmd.c_str());
            if (rootUser)
                cmd = "cat " + installDir + "/local/etc/" + parentOAMModuleName + "/" + files[i] + ".erydb >> " + fileName;
            else
                cmd = "sudo bash -c 'sudo cat " + installDir + "/local/etc/" + parentOAMModuleName + "/" + files[i] + ".erydb >> " + fileName + "'";
        }
        int rtnCode = system(cmd.c_str());
        if (WEXITSTATUS(rtnCode) != 0)
            cout << "Error Updating " + files[i] << endl;
        if (remote) {
            cmd = "ssh " + parentOAMModuleHostName + " rm -f " + installDir + "/local/ " + files[i] + "*.erydb > /dev/null 2>&1";
            system(cmd.c_str());
            cmd = "ssh " + parentOAMModuleHostName + " cp " + installDir + "/local/etc/" + parentOAMModuleName + "/" + files[i] + ".erydb " + installDir + "/local/. > /dev/null 2>&1";
            system(cmd.c_str());
        } else {
            cmd =  " rm -f " + installDir + "/local/ " + files[i] + "*.erydb > /dev/null 2>&1";
            system(cmd.c_str());
            cmd = " cp " + installDir + "/local/etc/" + parentOAMModuleName + "/" + files[i] + ".erydb " + installDir + "/local/. > /dev/null 2>&1";
            system(cmd.c_str());
        }
    }
    //check and do the amazon credentials file
    string fileName = HOME + "/.aws/credentials";
    ifstream oldFile(fileName.c_str());
    if (!oldFile)
        return allfound; 
    cmd = "cp " + fileName + " " + installDir + "/local/etc/ > /dev/null 2>&1";
    system(cmd.c_str());
    return allfound;
}


/*
 * Update ProcessConfig.xml file for a single server configuration
 * Change the 'um' and 'pm' to 'dm'
 */
bool updateProcessConfig(int serverTypeInstall) {
    vector <string> oldModule;
    string newModule;

    switch (serverTypeInstall) {
    case (oam::INSTALL_COMBINE_DM_UM_PM):
    {
        newModule = ">pm";
        oldModule.push_back(">um");
        oldModule.push_back(">dm");
        break;
    }
    case (oam::INSTALL_COMBINE_DM_UM):
    {
        newModule = ">um";
        oldModule.push_back(">dm");
        break;
    }
    case (oam::INSTALL_COMBINE_PM_UM):
    {
        newModule = ">pm";
        oldModule.push_back(">um");
        break;
    }
    }

    string fileName = installDir + "/etc/ProcessConfig.xml";

    //Save a copy of the original version
    string cmd = "/bin/cp -f " + fileName + " " + fileName + ".erydbSave > /dev/null 2>&1";
    system(cmd.c_str());

    ifstream oldFile(fileName.c_str());
    if (!oldFile) return true;

    vector <string> lines;
    char line[200];
    string buf;
    string newLine;
    string newLine1;

    while (oldFile.getline(line, 200)) {
        buf = line;
        newLine = line;

        std::vector<std::string>::iterator pt1 = oldModule.begin();
        for (; pt1 != oldModule.end(); pt1++) {
            int start = 0;
            while (true) {
                string::size_type pos = buf.find(*pt1, start);
                if (pos != string::npos) {
                    newLine = buf.substr(0, pos);
                    newLine.append(newModule);

                    newLine1 = buf.substr(pos + 3, 200);
                    newLine.append(newLine1);
                    start = pos + 3;
                } else {
                    buf = newLine;
                    start = 0;
                    break;
                }
            }
        }
        //output to temp file
        lines.push_back(buf);
    }

    oldFile.close();
    unlink(fileName.c_str());
    ofstream newFile(fileName.c_str());

    //create new file
    int fd = open(fileName.c_str(), O_RDWR | O_CREAT, 0664);

    copy(lines.begin(), lines.end(), ostream_iterator<string>(newFile, "\n"));
    newFile.close();

    close(fd);
    return true;
}

/*
 * Uncomment entry in erydb.xml
 */
bool uncommenterydbXml(string entry) {
    string fileName = installDir + "/etc/erydb.xml";

    ifstream oldFile(fileName.c_str());
    if (!oldFile) return true;

    vector <string> lines;
    char line[200];
    string buf;
    string newLine;

    string firstComment = "<!--";
    string lastComment = "--> ";

    while (oldFile.getline(line, 200)) {
        buf = line;

        string::size_type pos = buf.find(entry, 0);
        if (pos != string::npos) {
            pos = buf.find(firstComment, 0);
            if (pos == string::npos) {
                return true;
            } else {
                buf = buf.substr(pos + 4, 80);

                pos = buf.find(lastComment, 0);
                if (pos == string::npos) {
                    return true;
                } else {
                    buf = buf.substr(0, pos);
                }
            }
        }
        //output to temp file
        lines.push_back(buf);
    }

    oldFile.close();
    unlink(fileName.c_str());
    ofstream newFile(fileName.c_str());

    //create new file
    int fd = open(fileName.c_str(), O_RDWR | O_CREAT, 0664);

    copy(lines.begin(), lines.end(), ostream_iterator<string>(newFile, "\n"));
    newFile.close();

    close(fd);
    return true;
}

/*
 * Make makeRClocal to set mount scheduler
 */
bool makeRClocal(string moduleType, string moduleName, int IserverTypeInstall) {
    string fileName = installDir + "/local/etc/" + moduleName + "/rc.local.erydb";

    vector <string> lines;

    string mount1;
    string mount2
        ;
    switch (IserverTypeInstall) {
        case (oam::INSTALL_NORMAL):	// normal
        {
            if (moduleType == "um")
                mount1 = "/mysql\\/db/";
            else
                if (moduleType == "pm")
                    mount1 = "/erydb\\/data/";
                else
                    return true;
            break;
        }
        case (oam::INSTALL_COMBINE_DM_UM_PM):	// combined #1 - dm/um/pm
        {
            if (moduleType == "pm") {
                mount1 = "/mysql\\/db/";
                mount2 = "/erydb\\/data/";
            } else
                return true;
            break;
        }
        case (oam::INSTALL_COMBINE_DM_UM):	// combined #2 dm/um on a same server
        {
            if (moduleType == "um")
                mount1 = "/mysql\\/db/";
            else
                if (moduleType == "pm")
                    mount1 = "/erydb\\/data/";
                else
                    return true;
            break;
        }
        case (oam::INSTALL_COMBINE_PM_UM):	// combined #3 um/pm on a same server
        {
            if (moduleType == "pm") {
                mount1 = "/mysql\\/db/";
                mount2 = "/erydb\\/data/";
            } else
                return true;
            break;
        }
    }

    if (!mount1.empty()) {
        string line1 = "for scsi_dev in `mount | awk '" + mount1 + " {print $1}' | awk -F/ '{print $3}' | sed 's/[0-9]*$//'`; do";
        string line2 = "echo deadline > /sys/block/$scsi_dev/queue/scheduler";
        string line3 = "done";

        lines.push_back(line1);
        lines.push_back(line2);
        lines.push_back(line3);
        if (!mount2.empty()) {
            string line1 = "for scsi_dev in `mount | awk '" + mount2 + " {print $1}' | awk -F/ '{print $3}' | sed 's/[0-9]*$//'`; do";
            string line2 = "echo deadline > /sys/block/$scsi_dev/queue/scheduler";
            string line3 = "done";

            lines.push_back(line1);
            lines.push_back(line2);
            lines.push_back(line3);
        }

        unlink(fileName.c_str());

        if (lines.begin() == lines.end()) {
            string cmd = "touch " + fileName;
            system(cmd.c_str());
            return true;
        }

        ofstream newFile(fileName.c_str());

        //create new file
        int fd = open(fileName.c_str(), O_RDWR | O_CREAT, 0664);

        copy(lines.begin(), lines.end(), ostream_iterator<string>(newFile, "\n"));
        newFile.close();

        close(fd);

        return true;
    }
}
/*
 * createDbrootDirs
 */
bool createMetaDataDir(string DBRootStorageType) {
    string SystemSection = "SystemConfig";
    Config* sysConfig = Config::makeConfig();
    string dbrmroot = "";
    try {
        dbrmroot = sysConfig->getConfig(SystemSection, "DBRMRoot");
    } catch (const std::exception &exc) {
        std::cerr << exc.what() << std::endl;
    }
    string dbrmrootDir = dbrmroot;
    string::size_type pos = dbrmroot.find("/BRM_saves", 0);
    if (pos != string::npos)
        //get directory path
        dbrmrootDir = dbrmroot.substr(0, pos);

    if (dbrmrootDir == "") {
        dbrmrootDir = installDir + "/metadata/dbrm";
    }
    int rtnCode;
    string cmd; 
    // create system file directories
    cmd = "mkdir -p " + dbrmrootDir + " > /dev/null 2>&1";
    rtnCode = system(cmd.c_str());
    if (WEXITSTATUS(rtnCode) != 0) {
        cmd = "sudo mkdir -p " + dbrmrootDir + " > /dev/null 2>&1";
        rtnCode = system(cmd.c_str());
        if (WEXITSTATUS(rtnCode) != 0) {
            cout << endl << "Error: failed to make mount dbrm dir" << endl;
            return false;
        }
    }
    if (!rootUser) {
        cmd = "sudo chown -R " + USER + ":" + USER + " " + installDir + "/metadata/dbrm > /dev/null";
        system(cmd.c_str());
    }
    cmd = "chmod 755 -R " + dbrmrootDir + " > /dev/null 2>&1";
    system(cmd.c_str());
    return true;
}

/*
 * pkgCheck
 */
bool pkgCheck(string erydbPackage) {
    while (true) {
        string cmd = "ls " + erydbPackage + " > /tmp/erydbpkgs";
        system(cmd.c_str());
        string pkg = erydbPackage;
        string fileName = "/tmp/erydbpkgs";
        ifstream oldFile(fileName.c_str());
        if (oldFile) {
            oldFile.seekg(0, std::ios::end);
            int size = oldFile.tellg();
            if (size != 0) {
                oldFile.close();
                unlink(fileName.c_str());
                // pkgs found
                return true;
            }
        }
        cout << endl << " Error: can't locate " + pkg + " Package in directory " + HOME << endl << endl;
        if (noPrompting)
            exit(1);
        while (true) {
            pcommand = callReadline("Please place a copy of the erydb Packages in directory " + HOME + " and press <enter> to continue or enter 'exit' to exit the install > ");
            if (pcommand) {
                if (strcmp(pcommand, "exit") == 0) {
                    callFree(pcommand);
                    return false;
                }
                if (strlen(pcommand) == 0) {
                    callFree(pcommand);
                    break;
                }
                callFree(pcommand);
                cout << "Invalid entry, please re-enter" << endl;
                if (noPrompting)
                    exit(1);
                continue;
            }
            break;
        }
    }
    return true;
}

bool storageSetup(bool amazonInstall) {
    Oam oam;
    try {
        DBRootStorageType = sysConfig->getConfig(InstallSection, "DBRootStorageType");
    } catch (...) {
        cout << "ERROR: Problem getting DB Storage Data from the erydb System Configuration file" << endl;
        return false;
    }
    if (DBRootStorageType == "hdfs")
        hdfs = true;
    if (DBRootStorageType == "gluster")
        gluster = true;
    if (reuseConfig == "y") {
        cout << "===== Storage Configuration = " + DBRootStorageType + " =====" << endl << endl;
        if (hdfs) {
            //default
            DataFileEnvFile = "setenv-hdfs-20";
            try {
                DataFileEnvFile = sysConfig->getConfig("SystemConfig", "DataFileEnvFile");
            } catch (...) {
                DataFileEnvFile = "setenv-hdfs-20";
            }
            string DataFilePlugin = installDir + "/lib/libhdfs-20.so";
            try {
                DataFilePlugin = sysConfig->getConfig("SystemConfig", "DataFilePlugin");
            } catch (...) {
                DataFilePlugin = installDir + "/lib/libhdfs-20.so";
            }

            while (true) {
                cout << " Running HDFS Sanity Test (please wait):    ";
                cout.flush();
                string logdir("/var/log/erydb");
                if (access(logdir.c_str(), W_OK) != 0) logdir = "/tmp";
                string hdfslog = logdir + "/hdfsCheck.log1";

                string cmd = ". " + installDir + "/bin/" + DataFileEnvFile + ";" + installDir + "/bin/hdfsCheck " + DataFilePlugin + " > " + hdfslog + " 2>&1";
                system(cmd.c_str());
                if (oam.checkLogStatus(hdfslog, "All HDFS checks passed!")) {
                    cout << "  PASSED" << endl;

                    try {
                        sysConfig->setConfig("SystemConfig", "DataFilePlugin", DataFilePlugin);
                    } catch (...) {
                        cout << "ERROR: Problem setting DataFilePlugin in the erydb System Configuration file" << endl;
                        return false;
                    }

                    try {
                        sysConfig->setConfig("SystemConfig", "DataFileEnvFile", DataFileEnvFile);
                    } catch (...) {
                        cout << "ERROR: Problem setting DataFileEnvFile in the erydb System Configuration file" << endl;
                        return false;
                    }

                    if (!writeConfig(sysConfig)) {
                        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
                        return false;
                    }

                    return true;
                } else {
                    cout << "  FAILED (Tested with Hadoop Datafile Plugin File (" + DataFilePlugin + "), please re-enter or enter 'exit' to Investigate)" << endl << endl;

                    if (noPrompting)
                        exit(1);

                    prompt = "Enter the Hadoop Datafile Plugin File (" + DataFilePlugin + ") > ";
                    pcommand = callReadline(prompt.c_str());
                    if (pcommand) {
                        if (strlen(pcommand) > 0) DataFilePlugin = pcommand;
                        callFree(pcommand);
                    }

                    if (DataFilePlugin == "exit")
                        exit(1);

                    if (DataFilePlugin != installDir + "/lib/libhdfs-20.so")
                        DataFileEnvFile = "setenv-hdfs-12";
                    else
                        DataFileEnvFile = "setenv-hdfs-20";
                }
            }
        }
        return true;
    }

    cout << "===== Setup Storage Configuration =====" << endl << endl;
    //set UMStorageType
    string storageType;
    if (IserverTypeInstall != oam::INSTALL_COMBINE_DM_UM_PM && amazonInstall) {
        // get Frontend Data storage type
        cout << "----- Setup User Module erydb Data Storage Mount Configuration -----" << endl << endl;
        cout << "There are 2 options when configuring the storage: internal and external" << endl << endl;
        cout << "  'internal' -    This is specified when a local disk is used for the Data storage." << endl << endl;
        cout << "  'external' -    This is specified when the erydb Data directory is externally mounted." << endl << endl;
        try {
            UMStorageType = sysConfig->getConfig(InstallSection, "UMStorageType");
        } catch (...) {
            cout << "ERROR: Problem getting UM DB Storage Data from the erydb System Configuration file" << endl;
            return false;
        }
        while (true) {
            storageType = "1";
            if (UMStorageType == "external")
                storageType = "2";

            prompt = "Select the type of Data Storage [1=internal, 2=external] (" + storageType + ") > ";
            pcommand = callReadline(prompt.c_str());
            if (pcommand) {
                if (strlen(pcommand) > 0) storageType = pcommand;
                callFree(pcommand);
            }
            if (storageType == "1" || storageType == "2")
                break;
            cout << endl << "Invalid Entry, please re-enter" << endl << endl;
            if (noPrompting)
                exit(1);
        }

        if (storageType == "1")
            UMStorageType = "internal";
        else {
            cout << endl << "NOTE: The volume type. This can be gp2 for General Purpose  SSD,  io1  for" << endl;
            cout << "      Provisioned IOPS SSD, st1 for Throughput Optimized HDD, sc1 for Cold" << endl;
            cout << "      HDD, or standard for Magnetic volumes." << endl;

            UMStorageType = "external";

            cout << endl;
            try {
                oam.getSystemConfig("UMVolumeType", UMVolumeType);
            } catch (...) {
            }

            if (UMVolumeType.empty() || UMVolumeType == "")
                UMVolumeType = "gp2";

            while (true) {
                string prompt = "Enter EBS Volume Type (gp2, io1, sc1, st1, standard) : (" + UMVolumeType + ") > ";
                pcommand = callReadline(prompt);
                if (pcommand) {
                    if (strlen(pcommand) > 0) UMVolumeType = pcommand;
                    callFree(pcommand);
                }

                if (UMVolumeType == "standard" || UMVolumeType == "gp2" || UMVolumeType == "io1" || UMVolumeType == "sc1" || UMVolumeType == "st1")
                    break;
                else {
                    cout << endl << "Invalid Entry, please re-enter" << endl << endl;
                    if (noPrompting)
                        exit(1);
                }
            }

            //set UMVolumeType
            try {
                sysConfig->setConfig(InstallSection, "UMVolumeType", UMVolumeType);
            } catch (...) {
                cout << "ERROR: Problem setting UMVolumeType in the erydb System Configuration file" << endl;
                return false;
            }

            string minSize = "1";
            string maxSize = "16384";

            if (UMVolumeType == "io1")
                minSize = "4";

            if (UMVolumeType == "sc1" || UMVolumeType == "st1")
                minSize = "500";

            if (UMVolumeType == "standard")
                maxSize = "1024";

            cout << endl;
            try {
                oam.getSystemConfig("UMVolumeSize", UMVolumeSize);
            } catch (...) {
            }

            if (UMVolumeSize.empty() || UMVolumeSize == "" || UMVolumeSize == oam::UnassignedName)
                UMVolumeSize = minSize;

            while (true) {
                string prompt = "Enter EBS Volume storage size in GB: [" + minSize + "," + maxSize + "] (" + UMVolumeSize + ") > ";
                pcommand = callReadline(prompt);
                if (pcommand) {
                    if (strlen(pcommand) > 0) UMVolumeSize = pcommand;
                    callFree(pcommand);
                }

                if (atoi(UMVolumeSize.c_str()) < atoi(minSize.c_str()) || atoi(UMVolumeSize.c_str()) > atoi(maxSize.c_str())) {
                    cout << endl << "Invalid Entry, please re-enter" << endl << endl;
                    if (noPrompting)
                        exit(1);
                } else
                    break;
            }

            //set UMVolumeSize
            try {
                sysConfig->setConfig(InstallSection, "UMVolumeSize", UMVolumeSize);
            } catch (...) {
                cout << "ERROR: Problem setting UMVolumeSize in the erydb System Configuration file" << endl;
                return false;
            }


            if (UMVolumeType == "io1") {
                string minIOPS = UMVolumeSize;
                string maxIOPS = oam.itoa(atoi(UMVolumeSize.c_str()) * 30);

                cout << endl;
                try {
                    oam.getSystemConfig("UMVolumeIOPS", UMVolumeIOPS);
                } catch (...) {
                }

                if (UMVolumeIOPS.empty() || UMVolumeIOPS == "")
                    UMVolumeIOPS = maxIOPS;

                while (true) {
                    string prompt = "Enter EBS Volume IOPS: [" + minIOPS + "," + maxIOPS + "] (" + UMVolumeIOPS + ") > ";
                    pcommand = callReadline(prompt);
                    if (pcommand) {
                        if (strlen(pcommand) > 0) UMVolumeSize = pcommand;
                        callFree(pcommand);
                    }

                    if (atoi(UMVolumeSize.c_str()) < atoi(minIOPS.c_str()) || atoi(UMVolumeSize.c_str()) > atoi(maxIOPS.c_str())) {
                        cout << endl << "Invalid Entry, please re-enter" << endl << endl;
                        if (noPrompting)
                            exit(1);
                    } else
                        break;
                }

                //set UMVolumeIOPS
                try {
                    sysConfig->setConfig(InstallSection, "UMVolumeIOPS", UMVolumeIOPS);
                } catch (...) {
                    cout << "ERROR: Problem setting UMVolumeIOPS in the erydb System Configuration file" << endl;
                    return false;
                }
            }
        }

        try {
            sysConfig->setConfig(InstallSection, "UMStorageType", UMStorageType);
        } catch (...) {
            cout << "ERROR: Problem setting UMStorageType in the erydb System Configuration file" << endl;
            return false;
        }
    } else {
        try {
            sysConfig->setConfig(InstallSection, "UMStorageType", "internal");
        } catch (...) {
            cout << "ERROR: Problem setting UMStorageType in the erydb System Configuration file" << endl;
            return false;
        }
    }

    //check if gluster is installed
    {
        if (rootUser)
            system("which gluster > /tmp/gluster.log 2>&1");
        else
            system("sudo which gluster > /tmp/gluster.log 2>&1");

        ifstream in("/tmp/gluster.log");
        in.seekg(0, std::ios::end);
        int size = in.tellg();
        if (size == 0 || oam.checkLogStatus("/tmp/gluster.log", "no gluster"))
            // no gluster
            size = 0;
        else
            glusterInstalled = "y";
    }

    //check if hadoop is installed
    {
        system("which hadoop > /tmp/hadoop.log 2>&1");
        ifstream in1("/tmp/hadoop.log");
        in1.seekg(0, std::ios::end);
        int size = in1.tellg();
        if (size == 0 || oam.checkLogStatus("/tmp/hadoop.log", "no hadoop"))// no hadoop
            size = 0;
        else
            hadoopInstalled = "y";
    }

    //
    // get Backend Data storage type
    //

    // default to internal
    storageType = "1";
    if (DBRootStorageType == "external")
        storageType = "2";
    if (DBRootStorageType == "gluster")
        storageType = "3";
    if (DBRootStorageType == "hdfs")
        storageType = "4";

    cout << endl << "----- Setup Performance Module DBRoot Data Storage Mount Configuration -----" << endl << endl;

    if (glusterInstalled == "n" && hadoopInstalled == "n") {
        cout << "There are 2 options when configuring the storage: internal or external" << endl << endl;
        prompt = "Select the type of Data Storage [1=internal, 2=external] (" + storageType + ") > ";
    }

    if (glusterInstalled == "y" && hadoopInstalled == "n") {
        cout << "There are 3 options when configuring the storage: internal, external, or gluster" << endl << endl;
        prompt = "Select the type of Data Storage [1=internal, 2=external, 3=GlusterFS] (" + storageType + ") > ";
    }

    if (glusterInstalled == "n" && hadoopInstalled == "y") {
        cout << "There are 3 options when configuring the storage: internal, external, or hdfs" << endl << endl;
        prompt = "Select the type of Data Storage [1=internal, 2=external, 4=hdfs] (" + storageType + ") > ";
    }

    if (glusterInstalled == "y" && hadoopInstalled == "y") {
        cout << "There are 5 options when configuring the storage: internal, external, GlusterFS, or hdfs" << endl << endl;
        prompt = "Select the type of Data Storage [1=internal, 2=external, 3=GlusterFS, 4=hdfs] (" + storageType + ") > ";
    }

    cout << "  'internal' -    This is specified when a local disk is used for the DBRoot storage." << endl;
    cout << "                  High Availability Server Failover is not Supported in this mode" << endl << endl;
    cout << "  'external' -    This is specified when the DBRoot directories are mounted." << endl;
    cout << "                  High Availability Server Failover is Supported in this mode." << endl << endl;

    if (glusterInstalled == "y") {
        cout << "  'GlusterFS' -   This is specified when GlusterFS Data Redundancy is installed" << endl;
        cout << "                  and you want the DBRoot directories to be controlled by glusterfs." << endl;
        cout << "                  High Availability Server Failover is Supported in this mode." << endl << endl;
    }

    if (hadoopInstalled == "y") {
        cout << "  'hdfs' -        This is specified when hadoop is installed and you want the DBRoot" << endl;
        cout << "                  directories to be controlled by the Hadoop Distributed File System (HDFS)." << endl;
        cout << "                  High Availability Server Failover is Supported in this mode." << endl << endl;
    }

    while (true) {
        pcommand = callReadline(prompt.c_str());
        if (pcommand) {
            if (strlen(pcommand) > 0) storageType = pcommand;
            callFree(pcommand);
        }

        if (glusterInstalled == "n" && hadoopInstalled == "n") {
            if (storageType == "1" || storageType == "2")
                break;
            cout << endl << "Invalid Entry, please re-enter" << endl << endl;
            if (noPrompting)
                exit(1);
        }

        if (glusterInstalled == "y" && hadoopInstalled == "n") {
            if (storageType == "1" || storageType == "2" || storageType == "3")
                break;
            cout << endl << "Invalid Entry, please re-enter" << endl << endl;
            if (noPrompting)
                exit(1);
        }

        if (glusterInstalled == "n" && hadoopInstalled == "y") {
            if (storageType == "1" || storageType == "2" || storageType == "4") {
                break;
            }
            cout << endl << "Invalid Entry, please re-enter" << endl << endl;
            if (noPrompting)
                exit(1);
        }

        if (glusterInstalled == "y" && hadoopInstalled == "y") {
            if (storageType == "1" || storageType == "2" || storageType == "3" || storageType == "4")
                break;
            cout << endl << "Invalid Entry, please re-enter" << endl << endl;
            if (noPrompting)
                exit(1);
        }
    }

    switch (atoi(storageType.c_str())) {
        case (1):
        {
            DBRootStorageType = "internal";
            break;
        }
        case (2):
        {
            DBRootStorageType = "external";
            break;
        }
        case (3):
        {
            DBRootStorageType = "gluster";
            break;
        }
        case (4):
        {
            DBRootStorageType = "hdfs";
            break;
        }
    }

    //set DBRootStorageType
    try {
        sysConfig->setConfig(InstallSection, "DBRootStorageType", DBRootStorageType);
    } catch (...) {
        cout << "ERROR: Problem setting DBRootStorageType in the erydb System Configuration file" << endl;
        return false;
    }

    // if external and not amazon, print fstab note
    if (storageType == "2" && !amazonInstall) {
        cout << endl << "NOTE: For External configuration, the /etc/fstab should have been manually updated for the" << endl;
        cout << "      DBRoot mounts. Check the Installation Guide for further details" << endl << endl;

    }

    // if external and amazon, prompt for storage size
    if (storageType == "2" && amazonInstall) {
        cout << endl << "NOTE: The volume type. This can be gp2 for General Purpose  SSD,  io1  for" << endl;
        cout << "      Provisioned IOPS SSD, st1 for Throughput Optimized HDD, sc1 for Cold" << endl;
        cout << "      HDD, or standard for Magnetic volumes." << endl;

        cout << endl;
        try {
            oam.getSystemConfig("PMVolumeType", PMVolumeType);
        } catch (...) {
        }

        if (PMVolumeType.empty() || PMVolumeType == "")
            PMVolumeType = "gp2";

        while (true) {
            string prompt = "Enter EBS Volume Type (gp2, io1, sc1, st1, standard) : (" + PMVolumeType + ") > ";
            pcommand = callReadline(prompt);
            if (pcommand) {
                if (strlen(pcommand) > 0) PMVolumeType = pcommand;
                callFree(pcommand);
            }

            if (PMVolumeType == "standard" || PMVolumeType == "gp2" || PMVolumeType == "io1" || PMVolumeType == "sc1" || PMVolumeType == "st1")
                break;
            else {
                cout << endl << "Invalid Entry, please re-enter" << endl << endl;
                if (noPrompting)
                    exit(1);
            }
        }

        //set PMVolumeType
        try {
            sysConfig->setConfig(InstallSection, "PMVolumeType", PMVolumeType);
        } catch (...) {
            cout << "ERROR: Problem setting PMVolumeType in the erydb System Configuration file" << endl;
            return false;
        }

        cout << endl;

        string minSize = "1";
        string maxSize = "16384";

        if (PMVolumeType == "io1")
            minSize = "4";

        if (PMVolumeType == "sc1" || PMVolumeType == "st1")
            minSize = "500";

        if (PMVolumeType == "standard")
            maxSize = "1024";

        try {
            oam.getSystemConfig("PMVolumeSize", PMVolumeSize);
        } catch (...) {
        }

        if (PMVolumeSize.empty() || PMVolumeSize == "" || PMVolumeSize == oam::UnassignedName)
            PMVolumeSize = minSize;

        while (true) {
            string prompt = "Enter EBS Volume storage size in GB: [" + minSize + "," + maxSize + "] (" + PMVolumeSize + ") > ";
            pcommand = callReadline(prompt);
            if (pcommand) {
                if (strlen(pcommand) > 0) PMVolumeSize = pcommand;
                callFree(pcommand);
            }

            if (atoi(PMVolumeSize.c_str()) < atoi(minSize.c_str()) || atoi(PMVolumeSize.c_str()) > atoi(maxSize.c_str())) {
                cout << endl << "Invalid Entry, please re-enter" << endl << endl;
                if (noPrompting)
                    exit(1);
            } else
                break;
        }

        //set PMVolumeSize
        try {
            sysConfig->setConfig(InstallSection, "PMVolumeSize", PMVolumeSize);
        } catch (...) {
            cout << "ERROR: Problem setting PMVolumeSize in the erydb System Configuration file" << endl;
            return false;
        }


        if (PMVolumeType == "io1") {
            string minIOPS = PMVolumeSize;
            string maxIOPS = oam.itoa(atoi(PMVolumeSize.c_str()) * 30);

            cout << endl;
            try {
                oam.getSystemConfig("PMVolumeIOPS", PMVolumeIOPS);
            } catch (...) {
            }

            if (PMVolumeIOPS.empty() || PMVolumeIOPS == "" || PMVolumeIOPS == oam::UnassignedName)
                PMVolumeIOPS = maxIOPS;

            while (true) {
                string prompt = "Enter EBS Volume IOPS: [" + minIOPS + "," + maxIOPS + "] (" + PMVolumeIOPS + ") > ";
                pcommand = callReadline(prompt);
                if (pcommand) {
                    if (strlen(pcommand) > 0) PMVolumeIOPS = pcommand;
                    callFree(pcommand);
                }

                if (atoi(PMVolumeIOPS.c_str()) < atoi(minIOPS.c_str()) || atoi(PMVolumeIOPS.c_str()) > atoi(maxIOPS.c_str())) {
                    cout << endl << "Invalid Entry, please re-enter" << endl << endl;
                    if (noPrompting)
                        exit(1);
                } else
                    break;
            }

            //set PMVolumeIOPS
            try {
                sysConfig->setConfig(InstallSection, "PMVolumeIOPS", PMVolumeIOPS);
            } catch (...) {
                cout << "ERROR: Problem setting PMVolumeIOPS in the erydb System Configuration file" << endl;
                return false;
            }
        }

        //set DBRootStorageType
        try {
            sysConfig->setConfig(InstallSection, "PMVolumeSize", PMVolumeSize);
        } catch (...) {
            cout << "ERROR: Problem setting PMVolumeSize in the erydb System Configuration file" << endl;
            return false;
        }
    }

    // if gluster
    if (storageType == "3") {
        gluster = true;
        sysConfig->setConfig(InstallSection, "GlusterConfig", "y");
        sysConfig->setConfig("PrimitiveServers", "DirectIO", "n");
    } else {
        gluster = false;
        sysConfig->setConfig(InstallSection, "GlusterConfig", "n");
        sysConfig->setConfig("PrimitiveServers", "DirectIO", "y");
    }

    // if hadoop / hdfs
    if (storageType == "4") {
        hdfs = true;
        string DataFilePlugin = installDir + "/lib/libhdfs-20.so";
        try {
            DataFilePlugin = sysConfig->getConfig("SystemConfig", "DataFilePlugin");
        } catch (...) {
            DataFilePlugin = installDir + "/lib/libhdfs-20.so";
        }

        if (DataFilePlugin.empty() || DataFilePlugin == "")
            DataFilePlugin = installDir + "/lib/libhdfs-20.so";

        DataFileEnvFile = "setenv-hdfs-20";
        try {
            DataFileEnvFile = sysConfig->getConfig("SystemConfig", "DataFileEnvFile");
        } catch (...) {
            DataFileEnvFile = "setenv-hdfs-20";
        }

        if (DataFileEnvFile.empty() || DataFileEnvFile == "")
            DataFileEnvFile = "setenv-hdfs-20";

        cout << endl;
        while (true) {
            prompt = "Enter the Hadoop Datafile Plugin File (" + DataFilePlugin + ") > ";
            pcommand = callReadline(prompt.c_str());
            if (pcommand) {
                if (strlen(pcommand) > 0) DataFilePlugin = pcommand;
                callFree(pcommand);
            }
            if (DataFilePlugin == "exit")
                exit(1);
            if (DataFilePlugin != installDir + "/lib/libhdfs-20.so")
                DataFileEnvFile = "setenv-hdfs-12";

            ifstream File(DataFilePlugin.c_str());
            if (!File)
                cout << "Error: Hadoop Datafile Plugin File (" + DataFilePlugin + ") doesn't exist, please re-enter" << endl;
            else {
                cout << endl << " Running HDFS Sanity Test (please wait):    ";
                cout.flush();
                string logdir("/var/log/erydb");
                if (access(logdir.c_str(), W_OK) != 0) logdir = "/tmp";
                string hdfslog = logdir + "/hdfsCheck.log1";

                string cmd = installDir + "/bin/hdfsCheck " + DataFilePlugin + " > " + hdfslog + " 2>&1";
                system(cmd.c_str());
                if (oam.checkLogStatus(hdfslog, "All HDFS checks passed!")) {
                    cout << "  PASSED" << endl;
                    break;
                } else {
                    cout << "  FAILED (Tested with Hadoop Datafile Plugin File (" + DataFilePlugin + "), please re-enter or enter 'exit' to Investigate)" << endl << endl;
                }
            }
        }

        try {
            sysConfig->setConfig("SystemConfig", "DataFilePlugin", DataFilePlugin);
        } catch (...) {
            cout << "ERROR: Problem setting DataFilePlugin in the erydb System Configuration file" << endl;
            return false;
        }
        try {
            sysConfig->setConfig("SystemConfig", "DataFileEnvFile", DataFileEnvFile);
        } catch (...) {
            cout << "ERROR: Problem setting DataFileEnvFile in the erydb System Configuration file" << endl;
            return false;
        }
        try {
            sysConfig->setConfig("SystemConfig", "DataFileLog", "OFF");
        } catch (...) {
            cout << "ERROR: Problem setting DataFileLog in the erydb System Configuration file" << endl;
            return false;
        }
        try {
            sysConfig->setConfig("ExtentMap", "ExtentsPerSegmentFile", "1");
        } catch (...) {
            cout << "ERROR: Problem setting ExtentsPerSegmentFile in the erydb System Configuration file" << endl;
            return false;
        }
    } else {
        hdfs = false;

        try {
            sysConfig->setConfig("SystemConfig", "DataFilePlugin", "");
        } catch (...) {
            cout << "ERROR: Problem setting DataFilePlugin in the erydb System Configuration file" << endl;
            return false;
        }

        try {
            sysConfig->setConfig("SystemConfig", "DataFileEnvFile", "");
        } catch (...) {
            cout << "ERROR: Problem setting DataFileEnvFile in the erydb System Configuration file" << endl;
            return false;
        }

        if (!writeConfig(sysConfig)) {
            cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
            return false;
        }
    }

    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        return false;
    }

    return true;
}


void setSystemName() {
    //setup System Name
    try {
        systemName = sysConfig->getConfig(SystemSection, "SystemName");
    } catch (...) {
        systemName = oam::UnassignedName;
    }

    if (systemName.empty())
        systemName = oam::UnassignedName;

    prompt = "Enter System Name (" + systemName + ") > ";
    pcommand = callReadline(prompt.c_str());
    if (pcommand) {
        if (strlen(pcommand) > 0) systemName = pcommand;
        callFree(pcommand);
    }

    try {
        sysConfig->setConfig(SystemSection, "SystemName", systemName);
    } catch (...) {
        cout << "ERROR: Problem setting SystemName from the erydb System Configuration file" << endl;
        exit(1);
    }

    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        exit(1);
    }
}

/*
 * Copy fstab file
 */
bool copyFstab(string moduleName) {
    string cmd;
    if (rootUser)
        cmd = "/bin/cp -f /etc/fstab " + installDir + "/local/etc/" + moduleName + "/. > /dev/null 2>&1";
    else
        cmd = "/sudo bin/cp -f /etc/fstab " + installDir + "/local/etc/" + moduleName + "/. > /dev/null 2>&1";

    system(cmd.c_str());

    return true;
}


/*
 * Create a module file
 */
bool makeModuleFile(string moduleName, string parentOAMModuleName) {
    string fileName;
    //if (moduleName == parentOAMModuleName)
    //    fileName = installDir + "/local/module";
    //else
        fileName = installDir + "/local/etc/" + moduleName + "/module";
    unlink(fileName.c_str());
    //ofstream newFile(fileName.c_str());
    string cmd = "echo " + moduleName + " > " + fileName;
    system(cmd.c_str());
    //newFile.close();
    return true;
}

/*
 * Create a module file
 */
bool updateBash() {
    string fileName = HOME + "/.bashrc";
    ifstream newFile(fileName.c_str());
    if (hdfs) {
        //string cmd = "echo . " + installDir + "/bin/" + DataFileEnvFile + " >> " + fileName;
        string cmd = "scp " + installDir + "/bin/" + DataFileEnvFile + " /etc/profile.d/hdfs_env.sh " ;
        system(cmd.c_str());
        if (rootUser)
            cmd = "su - hdfs -c 'hadoop fs -mkdir -p " + installDir + ";hadoop fs -chown root:root " + installDir + "' >/dev/null 2>&1";
        else
            cmd = "sudo su - hdfs -c 'hadoop fs -mkdir -p " + installDir + ";hadoop fs -chown " + USER + ":" + USER + " " + installDir + "' >/dev/null 2>&1";
        system(cmd.c_str());
    }
    newFile.close();
    return true;

}

void offLineAppCheck() {
    //check for system startup type, process offline or online option
    cout << endl << "===== Setup Process Startup offline Configuration =====" << endl << endl;

    string systemStartupOffline;
    try {
        systemStartupOffline = sysConfig->getConfig(InstallSection, "SystemStartupOffline");
    } catch (...) {
        cout << "ERROR: Problem getting systemStartupOffline from the erydb System Configuration file" << endl;
        exit(1);
    }
    cout << endl;
    string temp = "y";
    while (true) {
        prompt = "Do you want the Database Processes started automatically at system startup [y,n] (y) > ";
        pcommand = callReadline(prompt.c_str());
        if (pcommand) {
            if (strlen(pcommand) > 0) temp = pcommand;
            callFree(pcommand);
            if (temp == "n" || temp == "y") {
                break;
            }
            cout << "Invalid Option, please re-enter" << endl;
            if (noPrompting)
                exit(1);
        } else
            break;
    }

    if (temp == "y")
        systemStartupOffline = "n";
    else
        systemStartupOffline = "y";
    try {
        sysConfig->setConfig(InstallSection, "SystemStartupOffline", systemStartupOffline);
    } catch (...) {
        cout << "ERROR: Problem setting systemStartupOffline in the erydb System Configuration file" << endl;
        exit(1);
    }
}

bool attachVolume(string instanceName, string volumeName, string deviceName, string dbrootPath) {
    Oam oam;

    //just return of debug set, called from amazonInstaller
    if (thread_remote_installer)
        return true;

    cout << "Checking if Volume " << volumeName << " is attached , please wait..." << endl;

    string status = oam.getEC2VolumeStatus(volumeName);
    if (status == "attached") {
        cout << "Volume " << volumeName << " is attached " << endl;
        cout << "Make sure it's device " << deviceName << " is mounted to DBRoot directory " << dbrootPath << endl;
        return true;
    }

    if (status != "available") {
        cout << "ERROR: Volume " << volumeName << " status is " << status << endl;
        cout << "Please resolve and re-run postConfigure" << endl;
        return false;
    } else {
        cout << endl;
        string temp = "y";
        while (true) {
            prompt = "Volume is unattached and available, do you want to attach it? [y,n] (y) > ";
            pcommand = callReadline(prompt.c_str());
            if (pcommand) {
                if (strlen(pcommand) > 0) temp = pcommand;
                callFree(pcommand);
                if (temp == "n" || temp == "y") {
                    break;
                }
                cout << "Invalid Option, please re-enter" << endl;
                if (noPrompting)
                    exit(1);
            } else
                break;
        }

        if (temp == "y") {
            cout << "Attaching, please wait..." << endl;
            if (oam.attachEC2Volume(volumeName, deviceName, instanceName)) {
                cout << "Volume " << volumeName << " is now attached " << endl;
                cout << "Make sure it's device " << deviceName << " is mounted to DBRoot directory " << dbrootPath << endl;
                return true;
            } else {
                cout << "ERROR: Volume " << volumeName << " failed to attach" << endl;
                cout << "Please resolve and re-run postConfigure" << endl;
                return false;
            }
        } else {
            cout << "Volume " << volumeName << " will need to be attached before completing the install" << endl;
            cout << "Please resolve and re-run postConfigure" << endl;
            return false;
        }
    }
}

bool singleServerDBrootSetup() {
    Oam oam;

    cout << endl;

    //get number of dbroots
    string moduledbrootcount = "ModuleDBRootCount1-3";
    unsigned int count;
    try {
        count = atoi(sysConfig->getConfig(ModuleSection, moduledbrootcount).c_str());
    } catch (...) {
        cout << "ERROR: Problem setting DBRoot count in the erydb System Configuration file" << endl;
        exit(1);
    }

    string dbrootList;

    for (unsigned int id = 1; id < count + 1; ) {
        string moduledbrootid = "ModuleDBRootID1-" + oam.itoa(id) + "-3";
        try {
            string dbrootid = sysConfig->getConfig(ModuleSection, moduledbrootid);

            if (dbrootid != oam::UnassignedName) {
                sysConfig->setConfig(ModuleSection, moduledbrootid, oam::UnassignedName);

                dbrootList = dbrootList + dbrootid;
                id++;
                if (id < count + 1)
                    dbrootList = dbrootList + ",";
            }
        } catch (...) {
            cout << "ERROR: Problem setting DBRoot ID in the erydb System Configuration file" << endl;
            exit(1);
        }
    }

    vector <string> dbroots;
    string tempdbrootList;

    while (true) {
        dbroots.clear();
        prompt = "Enter the list (Nx,Ny,Nz) or range (Nx-Nz) of DBRoot IDs assigned to module 'pm1' (" + dbrootList + ") > ";
        pcommand = callReadline(prompt.c_str());
        if (pcommand) {
            if (strlen(pcommand) > 0) {
                tempdbrootList = pcommand;
                callFree(pcommand);
            } else
                tempdbrootList = dbrootList;
        }

        if (tempdbrootList.empty())
            continue;

        //check for range
        int firstID;
        int lastID;
        string::size_type pos = tempdbrootList.find("-", 0);
        if (pos != string::npos) {
            firstID = atoi(tempdbrootList.substr(0, pos).c_str());
            lastID = atoi(tempdbrootList.substr(pos + 1, 200).c_str());

            if (firstID >= lastID) {
                cout << "Invalid Entry, please re-enter" << endl;
                continue;
            } else {
                for (int id = firstID; id < lastID + 1; id++) {
                    dbroots.push_back(oam.itoa(id));
                }
            }
        } else {
            boost::char_separator<char> sep(",");
            boost::tokenizer< boost::char_separator<char> > tokens(tempdbrootList, sep);
            for (boost::tokenizer< boost::char_separator<char> >::iterator it = tokens.begin();
                it != tokens.end();
                ++it) {
                dbroots.push_back(*it);
            }
        }
        break;
    }

    int id = 1;
    std::vector<std::string>::iterator it = dbroots.begin();
    for (; it != dbroots.end(); it++, ++id) {
        //store DBRoot ID
        string moduledbrootid = "ModuleDBRootID1-" + oam.itoa(id) + "-3";
        try {
            sysConfig->setConfig(ModuleSection, moduledbrootid, *it);
        } catch (...) {
            cout << "ERROR: Problem setting DBRoot ID in the erydb System Configuration file" << endl;
            exit(1);
        }

        string DBrootID = "DBRoot" + *it;
        string pathID = installDir + "/data" + *it;
        try {
            sysConfig->setConfig(SystemSection, DBrootID, pathID);
        } catch (...) {
            cout << "ERROR: Problem setting DBRoot in the erydb System Configuration file" << endl;
            return false;
        }
    }
    //store number of dbroots
    moduledbrootcount = "ModuleDBRootCount1-3";
    try {
        sysConfig->setConfig(ModuleSection, moduledbrootcount, oam.itoa(dbroots.size()));
    } catch (...) {
        cout << "ERROR: Problem setting DBRoot count in the erydb System Configuration file" << endl;
        exit(1);
    }
    //total dbroots on the system
    DBRootCount = DBRootCount + dbroots.size();
    if (!writeConfig(sysConfig)) {
        cout << "ERROR: Failed trying to update erydb System Configuration file" << endl;
        exit(1);
    }
    return true;
}

pthread_mutex_t THREAD_LOCK;

void remoteInstallThread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int rtnCode = system((data->command).c_str());
    if (WEXITSTATUS(rtnCode) != 0) {
        pthread_mutex_lock(&THREAD_LOCK);
        cout << endl << "Failure with a remote module install, check install log files in /tmp" << endl;
        exit(1);
    }
    // exit thread
    pthread_exit(0);
}

std::string launchInstance(ModuleIP moduleip) {
    Oam oam;

    //get module info
    string moduleName = moduleip.moduleName;
    string IPAddress = moduleip.IPaddress;
    string instanceName = oam::UnassignedName;

    //due to bad instances getting launched causing scp failures
    //have retry login around fail scp command where a instance will be relaunched
    int instanceRetry = 0;
    for (; instanceRetry < 5; instanceRetry++) {
        if (moduleName.find("um") == 0) {
            string UserModuleInstanceType;
            try {
                oam.getSystemConfig("UMInstanceType", UserModuleInstanceType);
            } catch (...) {}

            string UserModuleSecurityGroup;
            try {
                oam.getSystemConfig("UMSecurityGroup", UserModuleSecurityGroup);
            } catch (...) {}

            instanceName = oam.launchEC2Instance(moduleName, IPAddress, UserModuleInstanceType, UserModuleSecurityGroup);
        } else
            instanceName = oam.launchEC2Instance(moduleName, IPAddress);

        if (instanceName == "failed") {
            cout << " *** Failed to Launch an Instance for " + moduleName << ", will retry up to 5 times" << endl;
            continue;
        }

        cout << endl << "Launched Instance for " << moduleName << ": " << instanceName << endl;

        //give time for instance to startup
        sleep(60);

        string ipAddress = oam::UnassignedName;

        bool pass = false;
        for (int retry = 0; retry < 60; retry++) {
            //get IP Address of pm instance
            if (ipAddress == oam::UnassignedName || ipAddress == "stopped" || ipAddress == "No-IPAddress") {
                ipAddress = oam.getEC2InstanceIpAddress(instanceName);

                if (ipAddress == "stopped" || ipAddress == "No-IPAddress") {
                    sleep(5);
                    continue;
                }
            }

            pass = true;
            break;
        }

        if (!pass) {
            oam.terminateEC2Instance(instanceName);
            continue;
        }

        string autoTagging;
        oam.getSystemConfig("AmazonAutoTagging", autoTagging);

        if (autoTagging == "y") {
            string tagValue = systemName + "-" + moduleName;
            oam.createEC2tag(instanceName, "Name", tagValue);
        }

        break;
    }

    if (instanceRetry >= 5) {
        cout << " *** Failed to Successfully Launch Instance for " + moduleName << endl;
        return oam::UnassignedName;
    }

    return instanceName;
}



// vim:ts=4 sw=4:
