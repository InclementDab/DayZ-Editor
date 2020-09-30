@echo off

set QUERY="HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\bohemia interactive\addonbuilder"


for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "path"') do ( 
	set BUILDEREXE=%%b
)

for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "exe"') do ( 
	set BUILDEREXE="%BUILDEREXE%\%%b"
)

mkdir @DayZ-Editor\Addons

%BUILDEREXE% -clear "%CD%\Editor" "%CD%" -prefix=DayZEditor\Editor -packonly
%BUILDEREXE% -clear "%CD%\GUI" "%CD%" -prefix=DayZEditor\GUI -packonly
%BUILDEREXE% -clear "%CD%\Scripts" "%CD%" -prefix=DayZEditor\Scripts -packonly
%BUILDEREXE% -clear "%CD%\Sounds" "%CD%" -prefix=DayZEditor\Sounds -packonly


robocopy "%CD%" "@DayZ-Editor\Addons" "*.pbo" /mov