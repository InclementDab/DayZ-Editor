@echo off

set QUERY="HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\bohemia interactive\addonbuilder"


for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "path"') do ( 
	set BUILDEREXE=%%b
)

for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "exe"') do ( 
	set BUILDEREXE="%BUILDEREXE%\%%b"
)


%BUILDEREXE% "%CD%\Addons\editor" "%CD%\Addons" -prefix=DayZEditor\editor
%BUILDEREXE% "%CD%\Addons\gui" "%CD%\Addons" -prefix=DayZEditor\gui
REM %BUILDEREXE% "%CD%\Addons\scripts" "%CD%\Addons" -prefix=DayZEditor\scripts
