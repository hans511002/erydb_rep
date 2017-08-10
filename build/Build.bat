::@echo off

set branch=%1
set basedir=\EryDB_%branch%
IF "%branch%" == "" (
  set branch=develop
  set basedir=\EryDB
)
IF "%branch%" == "trunk" (
  set branch=develop
  set basedir=\EryDB
)
echo %branch%

echo building %basedir%

echo ======================================
echo updating mysql source
cd %basedir%\mysql
git checkout %branch%
git stash
git pull
git stash pop

echo ======================================
echo updating EryDB source
cd %basedir%\genii
git checkout %branch%
git pull origin_http %branch%
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

echo ======================================
echo Building mysql
cd %basedir%\mysql
MSBuild /M:8 /t:rebuild /p:Configuration="Release" /p:Platform="x64" mysql.sln
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

echo ======================================
echo Building EryDB
cd %basedir%\genii\build
MSBuild /M:8 /t:rebuild /p:Configuration="EnterpriseRelease" /p:Platform="x64" EryDB.sln
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

echo ======================================
echo Building infindb-ent
cd %basedir%\erydb-ent\build
call Build.bat
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
cd %basedir%\genii\build

echo ======================================
echo Building erydbVersion.txt
call BuilderydbVersion.bat

echo ======================================
echo Building the Standard Installer
makensis ..\utils\winport\erydb_64_standard.nsi
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER

echo ======================================
echo Building the Enterprise Installer
makensis ..\utils\winport\erydb_64_enterprise.nsi
IF %ERRORLEVEL% NEQ 0 GOTO ERROR_HANDLER
echo.

echo Build complete
GOTO QUIT

:ERROR_HANDLER
echo.
echo ======================================
echo Error occured. EryDB not built

cd %basedir%\genii\build
exit /B 1

:QUIT
echo ======================================
cd %basedir%\genii\build
echo    compiled at %date% %time%
echo nightly build complete

