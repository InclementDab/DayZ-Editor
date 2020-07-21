@echo off

set QUERY="HKEY_LOCAL_MACHINE\SOFTWARE\Bohemia Interactive\Dayz Tools\AddonBuilder"


for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "path"') do ( 
	set BUILDEREXE=%%b
)

for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "exe"') do ( 
	set BUILDEREXE="%BUILDEREXE%\%%b"
)


%BUILDEREXE% "%CD%\Addons\editor" "%CD%\Addons" -prefix=DayZEditor\editor -packonly
%BUILDEREXE% "%CD%\Addons\gui" "%CD%\Addons" -prefix=DayZEditor\gui -packonly
%BUILDEREXE% "%CD%\Addons\scripts" "%CD%\Addons" -prefix=DayZEditor\scripts -packonly
