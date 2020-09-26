@echo off

cd /D "%~dp0"

IF exist "P:\DayZEditor\" (
	echo Removing existing link P:\DayZEditor
	rmdir "P:\DayZEditor\"
)

echo Creating link P:\DayZEditor
mklink /J "P:\DayZEditor\" "%cd%\DayZEditor\"

echo Done