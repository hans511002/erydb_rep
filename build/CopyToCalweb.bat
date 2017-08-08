set dailydirname=genii-%DATE:~10,4%-%DATE:~4,2%-%DATE:~7,2%
xcopy /i /y \EryDB\genii\utils\winport\EryDB64.exe \\calweb\shared\\Iterations\Latest\packages\
xcopy /i /y \EryDB\genii\utils\winport\EryDB64.exe \\calweb\shared\\Iterations\nightly\%dailydirname%\packages\
xcopy /i /y \EryDB\genii\utils\winport\EryDB64-ent.exe \\calweb\shared\\Iterations\Latest\packages\
xcopy /i /y \EryDB\genii\utils\winport\EryDB64-ent.exe \\calweb\shared\\Iterations\nightly\%dailydirname%\packages\
