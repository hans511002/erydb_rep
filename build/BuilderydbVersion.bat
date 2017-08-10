for /f "delims= skip=1" %%x in (releasenum) do (set "%%x")
set packager="%USERNAME% <%USERNAME%@erydb.com>"
echo %packager%
set builddate="%DATE:~10,4%-%DATE:~4,2%-%DATE:~7,2% %TIME:~0,2%:%TIME:~3,2%:%TIME:~6,2%"
set buildmachine=%computername%
copy /Y .\erydbVersion.txt.in ..\utils\winport\erydbVersion.txt
sed -i  -e s/@@PACKAGER@@/%packager%/ ..\utils\winport\erydbVersion.txt
sed -i  -e s/@@BUILDDATE@@/%builddate%/ ..\utils\winport\erydbVersion.txt
sed -i  -e s/@@VERSION@@/%version%/ ..\utils\winport\erydbVersion.txt
sed -i  -e s/@@RELEASE@@/%release%/ ..\utils\winport\erydbVersion.txt
sed -i  -e s/@@BUILDMACHINE@@/%buildmachine%/ ..\utils\winport\erydbVersion.txt
:: For some unknown reason, sed removes permissions. Put them back.
icacls ..\utils\winport\erydbVersion.txt /grant SYSTEM:F administrators:F
cd ..
git status | grep "On branch" >> utils\winport\erydbVersion.txt
git log -1 | head -3 | egrep "^(commit|Date)" >> utils\winport\erydbVersion.txt

cd build
