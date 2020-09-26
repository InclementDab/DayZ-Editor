@echo off

set QUERY="HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\bohemia interactive\addonbuilder"


for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "path"') do ( 
	set BUILDEREXE=%%b
)

for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "exe"') do ( 
	set BUILDEREXE="%BUILDEREXE%\%%b"
)

%BUILDEREXE% "%CD%\editor" "%CD%" -prefix=DayZEditor\editor -packonly
%BUILDEREXE% "%CD%\gui" "%CD%" -prefix=DayZEditor\gui -packonly
%BUILDEREXE% "%CD%\scripts" "%CD%" -prefix=DayZEditor\scripts -packonly
%BUILDEREXE% "%CD%\sounds" "%CD%" -prefix=DayZEditor\sounds -packonly

