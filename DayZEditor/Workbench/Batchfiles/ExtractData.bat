@echo off
setlocal enableextensions enabledelayedexpansion
set projectfile=%~1
set userfile=%~2
set key=%~3
for /F "usebackq eol=: tokens=1,* delims==" %%i in ("!userfile!") do (
    set currkey=%%i
    set currval=%%j
    if "!key!"=="!currkey!" (
        echo.!currval!
        endlocal
        goto:eof
    )
)

for /F "usebackq eol=: tokens=1,* delims==" %%i in ("!projectfile!") do (
    set currkey=%%i
    set currval=%%j
    if "!key!"=="!currkey!" (
        echo.!currval!
        endlocal
        goto:eof
    )
)
endlocal