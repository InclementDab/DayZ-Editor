@echo off

cd /D "%~dp0"

set batchDirectory=%cd%

set /a failed=0

if exist ../project.cfg (
	echo Found the project.cfg
) else (
	echo Failed to find the project.cfg file, exitting.
	set /a failed=1
)

if exist ../user.cfg (
	echo Found the user.cfg
) else (
	echo Failed to find the user.cfg file, exitting.
	set /a failed=1
)

if %failed%==1 (
	endlocal

	echo Failed to package the mod.
	goto:eof
)

set workDrive=
set licensedModName=
set licensedModBuildDirectory=
set prefixLinkRoot=
set keyDirectory=
set keyName=
set dayzToolsPath=

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg WorkDrive') do (
	set workDrive=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg LicensedModName') do (
	set licensedModName=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg LicensedModBuildDirectory') do (
	set licensedModBuildDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg PrefixLinkRoot') do (
	set prefixLinkRoot=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg KeyDirectory') do (
	set keyDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg KeyName') do (
	set keyName=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg ToolsDirectory') do (
	set dayzToolsPath=%%a
)

REM @echo on

setlocal enableextensions enabledelayedexpansion

echo KeyDirectory is: "%keyDirectory%"
if "%keyDirectory%"=="" (
	set /a failed=1
	echo KeyDirectory parameter was not set in the project.cfg
)

echo KeyName is: "%keyName%"
if "%keyName%"=="" (
	set /a failed=1
	echo KeyName parameter was not set in the project.cfg
)

echo licensedModName is: "%licensedModName%"
if "%licensedModName%"=="" (
	set /a failed=1
	echo licensedModName parameter was not set in the project.cfg
)

echo WorkDrive is: "%workDrive%"
if "%workDrive%"=="" (
	set /a failed=1
	echo WorkDrive parameter was not set in the project.cfg
)

echo LicensedModBuildDirectory is: "%licensedModBuildDirectory%"
if "%licensedModBuildDirectory%"=="" (
	set /a failed=1
	echo licensedModBuildDirectory parameter was not set in the project.cfg
)

echo PrefixLinkRoot is: "%prefixLinkRoot%"
if "%prefixLinkRoot%"=="" (
	set /a failed=1
	echo PrefixLinkRoot parameter was not set in the project.cfg
)

echo Searching for Mikero Tools...
for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKCU\SOFTWARE\Mikero\depbo" /v "path" 2^>nul') do (set _MIKEDLL=%%B)
if not defined _MIKEDLL (
	for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKLM\SOFTWARE\Mikero\depbo" /v "path" 2^>nul') do (set _MIKEDLL=%%B)
	if not defined _MIKEDLL (
		echo Mikero Tools was not set in the registry path, trying default.
		set "_MIKEDLL=C:\Program Files (x86)\Mikero\DePboTools"
	) else (
		echo Found.
	)
) else (
	echo Found.
)

echo Searching for DayZ Tools...
for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKCU\Software\bohemia interactive\Dayz Tools" /v "Path" 2^>nul') do (set "_DAYZTOOLSPATH=%%B")
if not defined _DAYZTOOLSPATH (
	echo DayZ Tools was not set in the registry path, trying from configuration file.

	echo ToolsDirectory is: "%dayzToolsPath%"
	if "%dayzToolsPath%"=="" (
		set /a failed=1
		echo ToolsDirectory parameter was not set in the project.cfg
	) else (
		set _DAYZTOOLSPATH="%dayzToolsPath%"
	)
) else (
	echo ToolsDirectory is: "%_DAYZTOOLSPATH%"
)

set _DAYZTOOLSPATH=%_DAYZTOOLSPATH:"=%
set _MIKEDLL=%_MIKEDLL:"=%

if %failed%==1 (
	endlocal

	echo Failed to package the mod.
	goto:eof
)

set pboProject="%_MIKEDLL%\bin\pboProject.exe"
set signFile="%_DAYZTOOLSPATH%\Bin\DsUtils\DSSignFile.exe"

IF NOT exist "%licensedModBuildDirectory%%licensedModName%\Addons\" (
    echo %licensedModBuildDirectory%%licensedModName%\Addons\ does not exist
    pause
)

set folderToBuild=%~p1
set fuckThurston=

echo Copying over "%keyDirectory%%keyName%.bikey" to "%licensedModBuildDirectory%%licensedModName%\Keys\"
copy "%keyDirectory%%keyName%.bikey" "%licensedModBuildDirectory%%licensedModName%\Keys\" > nul

echo Packaging %licensedModName% PBO's

@echo off

cd /D "%workDrive%%prefixLinkRoot%\"

set pboName=%folderToBuild%
IF !pboName:~-1!==\ SET pboName=!pboName:~0,-1!
set pboName=!pboName:\%prefixLinkRoot%\=!
set pboName=!pboName:\=_!

set prefixName=%folderToBuild%
IF !prefixName:~-1!==\ SET prefixName=!prefixName:~0,-1!
set prefixName=!prefixName:\%prefixLinkRoot%\=!
set prefixName=%prefixLinkRoot%\!prefixName!

set sourcePath=%workDrive%!prefixName!

del %licensedModBuildDirectory%%licensedModName%\Addons\!pboName!.pbo
del %licensedModBuildDirectory%%licensedModName%\Addons\!pboName!.pbo.%keyName%.bisign

echo Building PBO: !pboName!.pbo
echo START /w %pboProject% %pboProject% +W -F +Stop -P -O +Z -E=dayz "%workDrive%!prefixName!" "+Mod=%licensedModBuildDirectory%%licensedModName%" "-Key"
START /w %pboProject% %pboProject% +W -F +Stop -P -O +Z -E=dayz "%workDrive%!prefixName!" "+Mod=%licensedModBuildDirectory%%licensedModName%" "-Key"

if not errorlevel 1 (
	set currentFolder=
	for %%I in ( %folderToBuild%\config.cpp\.. ) do (
		set "currentFolder=%%~nxI"
	)
	
	cd /D "%licensedModBuildDirectory%%licensedModName%\Addons\"

	echo Renaming PBO to %licensedModBuildDirectory%%licensedModName%\Addons\!pboName!.pbo
	rename "%licensedModBuildDirectory%%licensedModName%\Addons\!currentFolder!.pbo" "!pboName!.pbo"

	%signFile% "%keyDirectory%%keyName%.biprivatekey" "%licensedModBuildDirectory%%licensedModName%\Addons\!pboName!.pbo"
	goto end
) else (
	echo /////////////////////////////////////////////////////////////
	echo Something went wrong with %%D for !pboName!.pbo - this tool will not work until you fix it. Please check %workDrive%Temp\ for the issue should be the most recent packing/bin log
	echo /////////////////////////////////////////////////////////////
	pause
	goto end
)

:end
endlocal

echo "ENDED"
if EXIST %~p2 (
	exit
)