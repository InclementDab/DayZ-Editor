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

    cd %batchFileDirectory%
    goto:eof
)

set workbenchDirectory=
set workbenchEXE=
set workdrive=
set prefixLinkRoot=

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg WorkbenchDirectory') do (
    set workbenchDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg WorkbenchEXE') do (
    set workbenchEXE=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg WorkDrive') do (
	set workDrive=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg PrefixLinkRoot') do (
    set prefixLinkRoot=%%a
)

setlocal enableextensions enabledelayedexpansion

echo WorkbenchEXE is: "%workbenchEXE%"
if "%workbenchEXE%"=="" (
    set /a failed=1
    echo WorkbenchEXE parameter was not set in the project.cfg
)

echo WorkbenchDirectory is: "%workbenchDirectory%"
if "%workbenchDirectory%"=="" (
    set /a failed=1
    echo WorkbenchDirectory parameter was not set in the project.cfg
)

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

if %failed%==1 (
    endlocal

    echo Failed to package the mod.
    goto:eof
)

REM call CopyProject.bat

chdir /D "%workbenchDirectory%"
echo start %workbenchEXE% -gproj=%workDrive%%prefixLinkRoot%\Workbench\dayz.gproj
start %workbenchEXE% -gproj=%workDrive%%prefixLinkRoot%\Workbench\dayz.gproj

endlocal