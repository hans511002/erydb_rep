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

#include <iostream>
#include <string>
#include <unistd.h>
#include <io.h>
#include <sys\stat.h>
#include <erydbregistry.h>
#include <direct.h>
#include <malloc.h>
#include <fstream>
using namespace std;

#include "syncstream.h"

#include "fixup.h"
using namespace bootstrap;

#include "grepit.h"
using namespace winport;

namespace
{
	bool vFlg;

	string installDir;

	const size_t cmdLineLen = 2 * 1024;

	int fixPath()
	{
		int rc;
		string newDir = installDir + "\\bin";
		rc = _chdir(newDir.c_str());
		return rc;
	}

	int runIt(const string& pName)
	{
		char* cmdLine = (char*)_malloca(cmdLineLen);
		strncpy_s(cmdLine, cmdLineLen, pName.c_str(), pName.size());
		PROCESS_INFORMATION pInfo;
		ZeroMemory(&pInfo, sizeof(pInfo));
		STARTUPINFO sInfo;
		ZeroMemory(&sInfo, sizeof(sInfo));
		if (CreateProcess(0, cmdLine, 0, 0, false, 0, 0, 0, &sInfo, &pInfo) == 0)
		{
			_freea(cmdLine);
			return -1;
		}
		if (WaitForSingleObject(pInfo.hProcess, INFINITE) != WAIT_OBJECT_0)
		{
			_freea(cmdLine);
			return -1;
		}
		DWORD exitCode;
		GetExitCodeProcess(pInfo.hProcess, &exitCode);
		if (exitCode != 0)
		{
			_freea(cmdLine);
			return -1;
		}
		CloseHandle(pInfo.hProcess);
		_freea(cmdLine);
		return 0;
	}

	int installMySQL()
	{
		int rc;
		char* cmdLine = (char*)_malloca(cmdLineLen);
		sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysqld.exe --defaults-file=%s\\my.ini --bootstrap --loose-skip-innodb < %s\\etc\\win_setup_mysql_part1.sql",
			installDir.c_str(), installDir.c_str(), installDir.c_str());
		rc = system(cmdLine);
		_freea(cmdLine);
		return rc;
	}

	int installERYDBConn()
	{
		int rc;
		char* cmdLine = (char*)_malloca(cmdLineLen);
		sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\etc\\win_setup_mysql_part2.sql > nul 2>&1",
			installDir.c_str(), installDir.c_str(), installDir.c_str());
		rc = system(cmdLine);
		_freea(cmdLine);
		return rc;
	}

	int installERYDBConnStep2()
	{
		int rc = -1;
		char* cmdLine = (char*)_malloca(cmdLineLen);
		FILE* p;
		bool needAlterTable1 = false;
		bool needAlterTable2 = false;

		sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --execute=\"describe syscolumn;\" erydbsys 2>&1",
			installDir.c_str(), installDir.c_str());

		p = _popen(cmdLine, "rt");

		if (p == NULL) goto out;

		if (!grepit(syncstream::isyncstream(p), "^compressiontype[ \t].*"))
			needAlterTable1 = true;

		_pclose(p);

		p = _popen(cmdLine, "rt");

		if (p == NULL) goto out;

		if (!grepit(syncstream::isyncstream(p), "^nextvalue[ \t].*"))
			needAlterTable2 = true;

		_pclose(p);

		if (needAlterTable1 || needAlterTable2)
		{
			cout << "Checking EryDB components and system catalog for necessary upgrades..." << endl;

			//This script needs to be force-fed to mysqld and any errors ignored
			if (needAlterTable1)
			{
				sprintf_s(cmdLine, cmdLineLen,
					"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\etc\\win_setup_mysql_part3.sql > nul 2>&1",
					installDir.c_str(), installDir.c_str(), installDir.c_str());

				rc = system(cmdLine);
			}
			if (needAlterTable2)
			{
				sprintf_s(cmdLine, cmdLineLen,
					"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\etc\\win_setup_mysql_part3.1.sql > nul 2>&1",
					installDir.c_str(), installDir.c_str(), installDir.c_str());

				rc = system(cmdLine);
			}
		}
#ifndef SKIP_MYSQL_SETUP4
		sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\etc\\win_setup_mysql_part4.sql > nul 2>&1",
			installDir.c_str(), installDir.c_str(), installDir.c_str());
		rc = system(cmdLine);
		rc = 0;
#endif
		sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\etc\\win_setup_mysql_part5.sql > nul 2>&1",
			installDir.c_str(), installDir.c_str(), installDir.c_str());
		rc = system(cmdLine);
		rc = 0;

        // Install the user priority stored procedures
        sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\sql\\erysetuserpriority.sql > nul 2>&1",
			installDir.c_str(), installDir.c_str(), installDir.c_str());
		rc = system(cmdLine);
		rc = 0;

        sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\sql\\eryremoveuserpriority.sql > nul 2>&1",
			installDir.c_str(), installDir.c_str(), installDir.c_str());
		rc = system(cmdLine);
		rc = 0;

        sprintf_s(cmdLine, cmdLineLen,
			"%s\\bin\\mysql.exe --defaults-file=%s\\my.ini --user=root --force < %s\\sql\\eryshowprocesslist.sql > nul 2>&1",
			installDir.c_str(), installDir.c_str(), installDir.c_str());
		rc = system(cmdLine);
		rc = 0;

out:
		_freea(cmdLine);
		return rc;
	}

	int startupERYDB()
	{
		int rc;
		string cmd;
		cmd = installDir + "\\bin\\werydb.exe start";
		rc = runIt(cmd);
		return rc;
	}

	int installERYDBDB()
	{
		int rc;

		for (int retry = 0; retry < 5; retry++)
		{
			rc = installERYDBConn();
			if (rc == 0)
				break;
			Sleep(5 * (retry + 1) * 1000);
		}
		if (rc)
			return rc;

		rc = installERYDBConnStep2();
		if (rc)
			return rc;

		string cmd;
		cmd = installDir + "\\bin\\dbbuilder.exe 7";
		rc = runIt(cmd);
		return rc;
	}

	int upgradeEryDB()
	{
		int rc = 0;

		//We'll just have to blast these changes in...
		installERYDBConn();

		//Add new enterprise functions & new syscolumn columns
		rc = installERYDBConnStep2();
		if (rc)
			return rc;

		return rc;
	}

}

int main(int argc, char** argv)
{
	opterr = 0;
	vFlg = false;
	int c;

	while ((c = getopt(argc, argv, "v")) != EOF)
		switch (c)
		{
		case 'v':
			vFlg = true;
			break;
		case '?':
		default:
			break;
		}

	string mysqlPort("3306");

	if (argc - optind >= 1)
		mysqlPort = argv[optind++];

	installDir = ERYDBreadRegistry("", true);

	if (fixPath())
	{
		cerr << "Something went wrong trying to change to the install directory " << installDir << endl;
		return 1;
	}

	cout << "Running EryDB bootstrap installer..." << endl;

	cout << "Tuning configuration..." << endl;

	string moduleFile = installDir + "\\local\\module";
	if (_access(moduleFile.c_str(), F_OK) != 0)
	{
		ofstream mfs(moduleFile.c_str());
		if (!mfs.good())
		{
			cerr << "Something went wrong creating the module file" << endl;
			return 1;
		}
		mfs << "pm1" << endl;
		mfs.close();
	}

	if (fixupConfig(installDir, mysqlPort))
	{
		cerr << "Something went wrong fixing up a config file" << endl;
		return 1;
	}

	string mysqldb = installDir + "\\mysqldb\\mysql\\user.frm";
	struct _stat statbuf;

	if (_stat(mysqldb.c_str(), &statbuf) == 0)
	{
		cout << "Using existing MySQL database." << endl;
	}
	else
	{
		cout << "Installing empty MySQL database..." << endl;

		if (installMySQL())
		{
			cerr << "Something went wrong trying to setup MySQL" << endl;
			return 1;
		}
	}

	cout << "Starting EryDB..." << endl;

	if (startupERYDB())
	{
		cerr << "Something went wrong trying to start EryDB" << endl;
		return 1;
	}
	Sleep(5 * 1000);

	string cnffile = installDir + "\\data1\\000.dir";

	if (_stat(cnffile.c_str(), &statbuf) == 0)
	{
		cout << "Using existing EryDB database." << endl;
	}
	else
	{
		cout << "Installing EryDB components and system catalog..." << endl;

		if (installERYDBDB())
		{
			cerr << "Something went wrong trying to install EryDB components" << endl;
			return 1;
		}
	}

	if (upgradeEryDB())
	{
		cerr << "Something went wrong trying to upgrade EryDB components" << endl;
		return 1;
	}

	cout << "Done!" << endl;

	Sleep(3 * 1000);

	return 0;
}
