@echo off

set QUERY="HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\bohemia interactive\addonbuilder"


for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "path"') do ( 
	set BUILDEREXE=%%b
)

for /f "tokens=2* skip=2" %%a in ('reg query %QUERY%  /V "exe"') do ( 
	set BUILDEREXE="%BUILDEREXE%\%%b"
)

mkdir "%CD%\Addons\scripts\1_core"
mkdir "%CD%\Addons\scripts\3_game"
mkdir "%CD%\Addons\scripts\4_world"
mkdir "%CD%\Addons\scripts\5_mission"

robocopy /E "%CD%\1_core\dayzeditor" "%CD%\Addons\scripts\1_core"
robocopy /E "%CD%\3_game\dayzeditor" "%CD%\Addons\scripts\3_game"
robocopy /E "%CD%\4_world\dayzeditor" "%CD%\Addons\scripts\4_world"
robocopy /E "%CD%\5_mission\dayzeditor" "%CD%\Addons\scripts\5_mission"

%BUILDEREXE% "%CD%\Addons\editor" "%CD%\Addons" -prefix=DayZEditor\editor
%BUILDEREXE% "%CD%\Addons\gui" "%CD%\Addons" -prefix=DayZEditor\gui
%BUILDEREXE% "%CD%\Addons\scripts" "%CD%\Addons" -prefix=DayZEditor\scripts -packonly

rmdir /s "%CD%\Addons\scripts\1_core"
rmdir /s "%CD%\Addons\scripts\3_game"
rmdir /s "%CD%\Addons\scripts\4_world"
rmdir /s "%CD%\Addons\scripts\5_mission"
