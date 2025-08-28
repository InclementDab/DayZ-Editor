@echo off
SETLOCAL ENABLEDELAYEDEXPANSION

SET "drive=P:/"
:check
IF NOT EXIST %drive% (
	SET /p "drive=Enter Drive Letter (P:/):"
	goto check
)

REM Loop through all subdirectories in the current directory
FOR /D %%D IN (*) DO (
    REM Check for gproj file
    IF EXIST "%%D/Workbench/dayz.gproj" (
        REM Create a junction between the "Workbench" folder and P:\FolderName
        SET "junctionPath=%drive%\%%~nxD"
        ECHO Creating junction for "%%D" to "!junctionPath!"
        MKLINK /J "!junctionPath!" "%%D"
    )
)

for /d %%d in (Dependencies/*) do (
    echo Creating junction for "%%d" to "%drive%/%%d"
    mklink /j "%drive%/%%d" "%cd%/Dependencies/%%d"
    
)

ENDLOCAL
