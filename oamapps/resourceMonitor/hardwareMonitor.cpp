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

/***************************************************************************
 * $Id: hardwareMonitor.cpp 34 2006-09-29 21:13:54Z dhill $
 *
 *   Author: David Hill
 ***************************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cerrno>
#include <exception>
#include <stdexcept>

#include "liboamcpp.h"
#include "messagelog.h"
#include "messageobj.h"
#include "loggingid.h"
#include "alarmmanager.h"

using namespace std;
using namespace oam;
using namespace alarmmanager;
using namespace logging;

/**
 * @brief send alarm
 */
void sendAlarm(string alarmItem, ALARMS alarmID, int action);


/************************************************************************************************************
* @brief	main function
*
* purpose:	Get current hardware status and report alarms
*
* Parses file generated by the ipmitool
*
* pattern =  what it is | value | units | status | value 1 | value 2 | value 3 | value 4 | value 5 | value 6
* data(0) = what it is
* data(1) = value
* data(2) = units
* data(3) = status
* data(4)-data(9) = barrier values
*   data(4) - low non-recoverable, i.e. fatal
*   data(5) - low critical
*   data(6) - low warning
*   data(7) - high warning
*   data(8) - high critical
*   data(9) - high non-recoverable, i.e. fatal
*
************************************************************************************************************/
int main (int argc, char** argv)
{
	string data[10];
	string SensorName;
	string SensorValue;
    string Units;
	string SensorStatus;
	string lowFatal;
	string lowCritical;
	string lowWarning;
	string highWarning;
	string highCritical;
	string highFatal;
	char *p;

	// loop forever reading the hardware status
	while(TRUE)
	{
		int returnCode = system("ipmitool sensor list > /tmp/harwareMonitor.txt");
	
		if (returnCode) {
			// System error
			cout << "Error running ipmitool sensor list!!!" << endl;
			exit(-1);
		}
	
		// parse output file
	
		ifstream File ("/tmp/harwareMonitor.txt");
		if (!File){
			// System error
			cout << "Error opening /tmp/harwareMonitor.txt!!!" << endl;
			exit(-1);
		}
		
		char line[200];
		while (File.getline(line, 200))
		{
			// parse the line
			int f = 0;
			p = strtok(line,"|");
			while (p) 
			{
				data[f]=p;
				p = strtok (NULL, "|");
				f++;
			}
	
			if( f == 0 )
				// nothing on this line, skip
				continue;
	
			SensorName = data[0];
			SensorValue = data[1];
			Units = data[2];
			SensorStatus = data[3];
			lowFatal = data[4];
			lowCritical = data[5];
			lowWarning = data[6];
			highWarning = data[7];
			highCritical = data[8];
			highFatal = data[9];
	
			// check status and issue apporiate alarm if needed
			if ( (SensorStatus != "ok") && (SensorStatus != "nr") ) {
				// Status error, check for warning or critical levels

				if ( SensorValue >= highFatal )
					// issue critical alarm
					sendAlarm(SensorName, HARDWARE_HIGH, SET);

				else if ( (SensorValue < highFatal) && (SensorValue >= highCritical) )
					// issue major alarm
					sendAlarm(SensorName, HARDWARE_MED, SET);

				else if ( (SensorValue < highCritical ) && (SensorValue >= highWarning) )
					// issue minor alarm
					sendAlarm(SensorName, HARDWARE_LOW, SET);

				else if ( (SensorValue <= lowWarning) && (SensorValue > lowCritical) )
					// issue minor alarm
					sendAlarm(SensorName, HARDWARE_LOW, SET);

				else if ( (SensorValue <= lowCritical) && (SensorValue > lowFatal) )
					// issue major alarm
					sendAlarm(SensorName, HARDWARE_MED, SET);

				else if ( SensorValue <= lowFatal )
					// issue critical alarm
					sendAlarm(SensorName, HARDWARE_HIGH, SET);
			}
	
		} //end of parsing file while
		
		File.close();
		sleep(5);
	} //end of forever while loop
}
	
/******************************************************************************************
* @brief	sendAlarm
*
* purpose:	send a trap and log the process information
*
******************************************************************************************/
void sendAlarm(string alarmItem, ALARMS alarmID, int action)
{
	Oam oam;
	ALARMManager alarmMgr;
	// send alarm
	alarmMgr.sendAlarmReport(alarmItem.c_str(), alarmID, action);

	//Log this event 
	LoggingID lid;
	MessageLog ml(lid);
	Message msg;
	Message::Args args;
	args.add("Alarm action against ");
	args.add(alarmItem);
	if ( action == SET ) {
		args.add("Action type: SET");
	}
	else
	{
		args.add("Action type: CLEAR");
	}
	ml.logDebugMessage(msg);

	return;
}
