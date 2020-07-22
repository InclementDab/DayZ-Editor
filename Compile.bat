@echo off

set QUERY="HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\bohemia interactive\addonbuilder"


for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "path"') do ( 
	set BUILDEREXE=%%b
)

for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "exe"') do ( 
	set BUILDEREXE="%BUILDEREXE%\%%b"
)

REM %BUILDEREXE% "%CD%\Addons\editor" "%CD%\Addons" -prefix=DayZEditor\editor
REM %BUILDEREXE% "%CD%\Addons\gui" "%CD%\Addons" -prefix=DayZEditor\gui

REM %BUILDEREXE% "%CD%\Addons\scripts" "%CD%\Addons" -prefix=DayZEditor\scripts

MakePBO -U %CD%\Addons\editor
MakePBO -U %CD%\Addons\gui