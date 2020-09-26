@echo off

setlocal enableextensions enabledelayedexpansion

cd /D "%~dp0"
set CurrentDir=%CD%

set WorkbenchProfile=P:\profiles\Workbench
set ProjectFile=DayZ-Editor.gproj


taskkill /F /IM workbenchApp.exe

for /f "tokens=2* skip=2" %%a in ('REG QUERY "HKEY_CURRENT_USER\Software\Bohemia Interactive\Dayz Tools" /v "path"') do (
	set "TOOLS_DIR=%%b"
)


for /f "tokens=2* skip=2" %%a in ('REG QUERY "HKEY_CURRENT_USER\Software\Bohemia Interactive\Dayz Tools\Workbench\Main" /v "sourceDataDir"') do (
	set "SOURCE_DIR=%%b"
)

if "%TOOLS_DIR%"=="" (
	echo DayZ Tools not installed!
	pause
	exit
)

set "WORKBENCH_DIR=%TOOLS_DIR%\Bin\Workbench"
set "GPROJ_FILE=%SOURCE_DIR%%ProjectFile%"

echo Copying %CurrentDir%\%ProjectFile% to %GPROJ_FILE%
xcopy /Y /I "%CurrentDir%\%ProjectFile%" "%GPROJ_FILE%"

cd /d %WORKBENCH_DIR%
start workbenchApp.exe -gproj=%GPROJ_FILE% -profiles=%WorkbenchProfile% -noPause -filePatching -checkInstance -debugger -forcedebugger -freezecheck -dologs -showscripterrors





