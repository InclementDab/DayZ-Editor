@echo off

cd /D "%~dp0"

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

set rootDirectory=%cd%\..\..\..\
set workDrive=
set prefixLinkRoot=
set workbenchDirectory=
set gameDirectory=
set serverDirectory=
set modName=
set modBuildDirectory=

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg WorkDrive') do (
    set workDrive=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg PrefixLinkRoot') do (
	set prefixLinkRoot=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg GameDirectory') do (
    set gameDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg ServerDirectory') do (
    set serverDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg ModName') do (
	set modName=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg ModBuildDirectory') do (
	set modBuildDirectory=%%a
)

setlocal enableextensions enabledelayedexpansion

echo WorkDrive is: "%workDrive%"
if "%workDrive%"=="" (
    set /a failed=1
    echo WorkDrive parameter was not set in the project.cfg
)

echo PrefixLinkRoot is: "%prefixLinkRoot%"
if "%prefixLinkRoot%"=="" (
	set /a failed=1
	echo PrefixLinkRoot parameter was not set in the project.cfg
)

echo GameDirectory is: "%gameDirectory%"
if "%gameDirectory%"=="" (
    set /a failed=1
    echo GameDirectory parameter was not set in the project.cfg
)

echo ServerDirectory is: "%serverDirectory%"
if "%serverDirectory%"=="" (
    set /a failed=1
    echo ServerDirectory parameter was not set in the project.cfg
)

echo ModName is: "%modName%"
if "%modName%"=="" (
	set /a failed=1
	echo ModName parameter was not set in the project.cfg
)

echo ModBuildDirectory is: "%modBuildDirectory%"
if "%modBuildDirectory%"=="" (
	set /a failed=1
	echo ModBuildDirectory parameter was not set in the project.cfg
)

if %failed%==1 (
    endlocal

    echo Failed to package the mod.
    goto:eof
)

IF exist "%workDrive%%prefixLinkRoot%\" (
	echo Removing existing link %workDrive%%prefixLinkRoot%\
	rmdir "%workDrive%%prefixLinkRoot%\"
)

echo Creating link %workDrive%%prefixLinkRoot%\
mklink /J "%workDrive%%prefixLinkRoot%\" "%rootDirectory%%prefixLinkRoot%\"

for /f "tokens=*" %%D in ('dir /b "%rootDirectory%\Missions\"') do (
    set missionFolder=%%~D

    IF exist "%gameDirectory%\Missions\!missionFolder!\" (
        echo.Please remove existing link: "%gameDirectory%\Missions\!missionFolder!\"
        REM rmdir /s /q "%gameDirectory%\Missions\!missionFolder!\"
    ) else (
        echo.Creating link "%gameDirectory%\Missions\!missionFolder!\"
        mklink /J "%gameDirectory%\Missions\!missionFolder!\" "%rootDirectory%\Missions\\!missionFolder!\"
    )

    IF exist "%serverDirectory%\MPMissions\!missionFolder!\" (
        echo.Please remove existing link: "%serverDirectory%\MPMissions\!missionFolder!\"
        REM rmdir /s /q "%serverDirectory%\MPMissions\!missionFolder!\"
    ) else (
        echo.Creating link "%serverDirectory%\MPMissions\!missionFolder!\"
        mklink /J "%serverDirectory%\MPMissions\!missionFolder!\" "%rootDirectory%\Missions\\!missionFolder!\"
    )
)

endlocal