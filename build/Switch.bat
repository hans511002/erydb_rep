::@echo off
IF NOT EXIST C:\EryDB_3.6\NUL GOTO SWITCH_to_trunk
IF NOT EXIST C:\EryDB_4.0\NUL GOTO SWITCH_to_3.6
IF NOT EXIST C:\EryDB_trunk\NUL GOTO SWITCH_to_4.0

:SWITCH_to_trunk
echo switching to trunk
REN "\EryDB" "EryDB_3.6"
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
REN "\EryDB_trunk" "EryDB"
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
echo Switched to EryDB trunk
GOTO COMPLETE

:SWITCH_to_3.6
echo switching to 3.6
REN "\EryDB" "EryDB_4.0"
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
REN "\EryDB_3.6" "EryDB"
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
echo Switched to EryDB 3.6
GOTO COMPLETE

:SWITCH_to_4.0
echo switching to 4.0
REN "\EryDB" "EryDB_trunk"
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
REN "\EryDB_4.0" "EryDB"
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
echo Switched to EryDB 4.0
GOTO COMPLETE

:ERROR_HANDLER
echo failed to switch

:COMPLETE
