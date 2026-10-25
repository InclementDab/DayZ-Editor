@echo off
set repository="%cd%"

rem Loop through all directories in the current directory
for /d %%D in (*) do (
    if exist "%%D\Workbench\" (
        cd /d "%%D\Workbench"
        goto :foundwb
    )
)

echo Workbench folder not found

:foundwb

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

start "" /b "%dayz_tools%\Bin\Workbench\workbenchApp.exe" -filePatching -doLogs "-repository=%repository%" "-prefix=DayZEditor" "-mod=C:\Dev\proj\DayZ-Dabs-Framework;C:\Dev\proj\DayZ-CommunityFramework;C:\Dev\proj\DayZ-Editor"

exit
