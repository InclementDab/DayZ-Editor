@echo off

set "ADDONS=P:\DayZ_Server\dev\DayZEditor\Addons"

cd %ADDONS%

MakePBO -U -P editor
MakePBO -P gui
MakePBO -P scripts

