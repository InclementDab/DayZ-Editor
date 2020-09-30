@echo off

set QUERY="HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\bohemia interactive\addonbuilder"


for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "path"') do ( 
	set BUILDEREXE=%%b
)

for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "exe"') do ( 
	set BUILDEREXE="%BUILDEREXE%\%%b"
)

%BUILDEREXE% "%CD%\CF_Scripts" "%CD%" -prefix=DayZEditor\CF_Scripts -packonly
%BUILDEREXE% "%CD%\Editor" "%CD%" -prefix=DayZEditor\Editor -packonly
%BUILDEREXE% "%CD%\GUI" "%CD%" -prefix=DayZEditor\GUI -packonly
%BUILDEREXE% "%CD%\Scripts" "%CD%" -prefix=DayZEditor\Scripts -packonly
%BUILDEREXE% "%CD%\Sounds" "%CD%" -prefix=DayZEditor\Sounds -packonly


