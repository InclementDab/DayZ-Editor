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
set gameDirectory=
set serverDirectory=

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg WorkbenchDirectory') do (
    set workbenchDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg GameDirectory') do (
    set gameDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg ServerDirectory') do (
    set serverDirectory=%%a
)

setlocal enableextensions enabledelayedexpansion

echo WorkbenchDirectory is: "%workbenchDirectory%"
if "%workbenchDirectory%"=="" (
    set /a failed=1
    echo WorkbenchDirectory parameter was not set in the project.cfg
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

if %failed%==1 (
    endlocal

    echo Failed to package the mod.
    goto:eof
)

echo Copying "%cd%\..\dayz.gproj" to "%workbenchDirectory%dayz.gproj"
copy "%cd%\..\dayz.gproj" "%workbenchDirectory%dayz.gproj"
echo Copying "%cd%\..\dayz.gproj" to "%gameDirectory%dayz.gproj"
copy "%cd%\..\dayz.gproj" "%gameDirectory%dayz.gproj"
echo Copying "%cd%\..\dayz.gproj" to "%serverDirectory%dayz.gproj"
copy "%cd%\..\dayz.gproj" "%serverDirectory%dayz.gproj"

endlocal