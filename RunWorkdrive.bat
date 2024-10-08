@echo off
set repository="%cd%"

cd /d "%~dp0DayZEditor\Workbench"

taskkill /f /im "workbenchApp.exe"

for /f "tokens=2,*" %%a in ('reg query "HKCU\SOFTWARE\Bohemia Interactive\Dayz Tools" /v "path" 2^>nul') do (
    set "dayz_tools=%%b"
)

:: Check if the last part of the dayz_tools path is "DayZ Tools"
if "%dayz_tools:~-10%"=="DayZ Tools" (
    set "profile_path=%homedrive%%homepath%\Documents\DayZ"
) else (
    set "profile_path=%homedrive%%homepath%\Documents\DayZ Exp"
)

start "" /b "%dayz_tools%\Bin\Workbench\workbenchApp.exe" "-profiles=%profile_path% -repository=\"%repository%\""

exit
