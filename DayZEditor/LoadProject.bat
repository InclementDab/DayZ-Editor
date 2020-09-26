@echo off

setlocal enableextensions enabledelayedexpansion

cd /D "%~dp0"
set CURRENT_DIR=%CD%

set WorkbenchProfile=P:\profiles\Workbench
set ProjectFile=DayZ-Editor.gproj

taskkill /F /IM workbenchApp.exe

for /f "tokens=2* skip=2" %%a in ('REG QUERY "HKEY_CURRENT_USER\Software\Bohemia Interactive\Dayz Tools" /v "path"') do (
	set "TOOLS_DIR=%%b"
)

REM TBD...
REM for /f "tokens=2* skip=2" %%a in ('REG QUERY "HKEY_CURRENT_USER\Software\Bohemia Interactive\Dayz Tools\Workbench\Main" /v "sourceDataDir"') do (
REM 	set "SOURCEDATA_DIR=%%b"
REM )


if "%TOOLS_DIR%"=="" (
	echo DayZ Tools not installed!
	pause
	exit
)

set "WORKBENCH_DIR=%TOOLS_DIR%\Bin\Workbench"

cd /d %WORKBENCH_DIR%
start workbenchApp.exe -gproj=%CURRENT_DIR%\%ProjectFile% -profiles=%WorkbenchProfile% -noPause -filePatching -checkInstance -debugger -forcedebugger -freezecheck -dologs -showscripterrors







