@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

REM Loop through all subdirectories in the current directory
FOR /D %%D IN (*) DO (
    REM Check for gproj file
    IF EXIST "%%D\Workbench\dayz.gproj" (
        REM Create a junction between the "Workbench" folder and P:\FolderName
        SET "junctionPath=P:\%%~nxD"
        ECHO Creating junction for "%%D" to "!junctionPath!"
        MKLINK /J "!junctionPath!" "%%D"
    )
)

ENDLOCAL
