@echo off

cd /D "%~dp0"

set /a failed=0

if exist ../project.cfg (
	echo Found the project.cfg
) else (
	echo Failed to find the project.cfg file, exitting.
	set /a failed=1
)

if exist ../user.cfg (
	echo Found the user.cfg
) else (
	echo Failed to find the user.cfg file, exitting.
	set /a failed=1
)

if %failed%==1 (
	endlocal

	echo Failed to package the mod.
	goto:eof
)

set githubDirectory=%cd%\
set workbenchDataDirectory=%githubDirectory%Workbench\
set toolsDirectory=%workbenchDataDirectory%Tools\

set workDrive=
set modName=
set modBuildDirectory=
set prefixLinkRoot=
set keyDirectory=
set keyName=

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg WorkDrive') do (
	set workDrive=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg ModName') do (
	set modName=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg ModBuildDirectory') do (
	set modBuildDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg PrefixLinkRoot') do (
	set prefixLinkRoot=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg KeyDirectory') do (
	set keyDirectory=%%a
)

for /f "delims=" %%a in ('call ExtractData.bat ../project.cfg ../user.cfg KeyName') do (
	set keyName=%%a
)

REM @echo on

setlocal enableextensions enabledelayedexpansion

echo KeyDirectory is: "%keyDirectory%"
if "%keyDirectory%"=="" (
	set /a failed=1
	echo KeyDirectory parameter was not set in the project.cfg
)

echo KeyName is: "%keyName%"
if "%keyName%"=="" (
	set /a failed=1
	echo KeyName parameter was not set in the project.cfg
)

echo ModName is: "%modName%"
if "%modName%"=="" (
	set /a failed=1
	echo ModName parameter was not set in the project.cfg
)

echo WorkDrive is: "%workDrive%"
if "%workDrive%"=="" (
	set /a failed=1
	echo WorkDrive parameter was not set in the project.cfg
)

echo ModBuildDirectory is: "%modBuildDirectory%"
if "%modBuildDirectory%"=="" (
	set /a failed=1
	echo ModBuildDirectory parameter was not set in the project.cfg
)

echo PrefixLinkRoot is: "%prefixLinkRoot%"
if "%prefixLinkRoot%"=="" (
	set /a failed=1
	echo PrefixLinkRoot parameter was not set in the project.cfg
)

echo Searching for Mikero Tools...
for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKCU\SOFTWARE\Mikero\depbo" /v "path" 2^>nul') do (set _MIKEDLL=%%B)
if not defined _MIKEDLL (
	for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKLM\SOFTWARE\Mikero\depbo" /v "path" 2^>nul') do (set _MIKEDLL=%%B)
	if not defined _MIKEDLL (
		set /a failed=1
		echo Mikero Tools was not set in the registry path.
	) else (
		echo Found.
	)
) else (
	echo Found.
)

echo Searching for DayZ Tools...
for /F "Tokens=2* skip=2" %%A In ('REG QUERY "HKCU\Software\bohemia interactive\Dayz Tools" /v "Path" 2^>nul') do (set _DAYZTOOLSPATH=%%B)
if not defined _DAYZTOOLSPATH (
	set /a failed=1
	echo DayZ Tools was not set in the registry path.
) else (
	echo Found.
)

if %failed%==1 (
	endlocal

	echo Failed to package the mod.
	goto:eof
)

CALL Exit.bat

set makePBO="%_MIKEDLL%\bin\MakePbo.exe"
set binarize="%_DAYZTOOLSPATH%\Bin\Binarize\binarize.exe"
set signFile="%_DAYZTOOLSPATH%\Bin\DsUtils\DSSignFile.exe"

IF exist "%modBuildDirectory%%modName%\" (
	echo Removing folder "%modBuildDirectory%%modName%\"
	rmdir /S /Q "%modBuildDirectory%%modName%\"
)

IF NOT exist "%modBuildDirectory%%modName%\" (
	echo Creating folder "%modBuildDirectory%%modName%\"
	mkdir "%modBuildDirectory%%modName%\"
)

IF NOT exist "%modBuildDirectory%%modName%\Keys\" (
	echo Creating folder "%modBuildDirectory%%modName%\Keys\"
	mkdir "%modBuildDirectory%%modName%\Keys\"
)

IF NOT exist "%modBuildDirectory%%modName%\Addons\" (
	echo Creating folder "%modBuildDirectory%%modName%\Addons\"
	mkdir "%modBuildDirectory%%modName%\Addons\"
)

echo Copying over "%workDrive%%prefixLinkRoot%\mod.cpp" to "%modBuildDirectory%%modName%\"
copy "%workDrive%%prefixLinkRoot%\mod.cpp" "%modBuildDirectory%%modName%\" > nul

echo Copying over "%keyDirectory%\%keyName%.bikey" to "%modBuildDirectory%%modName%\Keys\"
copy "%keyDirectory%\%keyName%.bikey" "%modBuildDirectory%%modName%\Keys\" > nul

IF exist "%workDrive%Temp\%prefixLinkRoot%\" (
	echo Removing folder "%workDrive%Temp\%prefixLinkRoot%\"
	del /F /S /Q "%workDrive%Temp\%prefixLinkRoot%\">NUL
	rmdir /S /Q "%workDrive%Temp\%prefixLinkRoot%\">NUL
)

IF NOT exist "%workDrive%Temp\%prefixLinkRoot%\" (
	echo Copying folder "%workDrive%%prefixLinkRoot%\" to "%workDrive%Temp\%prefixLinkRoot%\"
	xcopy "%workDrive%%prefixLinkRoot%" "%workDrive%Temp\%prefixLinkRoot%\" /S /H /Y /Q /EXCLUDE:excludes.txt
)

echo Packaging %modName% PBO's

cd /D "%workDrive%%prefixLinkRoot%\"
cd /D "%workDrive%Temp\%prefixLinkRoot%\"

for /R %%D in ( config.cpp ) do (
	echo D %%~dD%%~pDconfig.cpp
	IF EXIST "%%~dD%%~pDconfig.cpp" (
		IF NOT EXIST "%%~dD%%~pD..\config.cpp" (
			IF NOT EXIST "%%~dD%%~pD..\..\config.cpp" (
				IF NOT EXIST "%%~dD%%~pD..\..\..\config.cpp" (
					IF NOT EXIST "%%~dD%%~pD..\..\..\..\config.cpp" (
						IF NOT EXIST "%%~dD%%~pD..\..\..\..\..\config.cpp" (
							IF NOT EXIST "%%~dD%%~pD..\..\..\..\..\..\config.cpp" (
								set pboName=%%~pD
								IF !pboName:~-1!==\ SET pboName=!pboName:~0,-1!
								set pboName=!pboName:\Temp\%prefixLinkRoot%\=!
								set pboName=!pboName:\=_!
								set pboName="%modBuildDirectory%%modName%\Addons\!pboName!.pbo

								set prefixName=%%~pD
								IF !prefixName:~-1!==\ SET prefixName=!prefixName:~0,-1!


								set prefixName=!prefixName:\Temp\%prefixLinkRoot%\=!
								set prefixName=%prefixLinkRoot%\!prefixName!

								set targetPath=%workDrive%Temp\!prefixName!
								set sourcePath=%workDrive%!prefixName!

								set prefixName=!prefixName:\=/!

								set sourceFolder=%%~dpD
								IF !sourceFolder:~-1!==\ SET sourceFolder=!sourceFolder:~0,-1!


								set outputLogFile=%workDrive%Temp\!prefixName!

								echo Prefix:		!prefixName!
								echo Config:		%%~fD
								echo Source Folder:	!sourceFolder!
								echo Target PBO:	!pboName!
								echo Source Path:	!sourcePath!
								echo Target Path:	!targetPath!

								REM WRP Binarization
								REM echo %binarize% -addon=%sourcePath% "-binPath=%_DAYZTOOLSPATH%\Bin\Binarize" "-textures=!targetPath!" !sourcePath! !targetPath! *.wrp !outputLogFile!.wlg
								REM %binarize% -addon=%sourcePath% "-binPath=%_DAYZTOOLSPATH%\Bin\Binarize" "-textures=!targetPath!" !sourcePath! !targetPath! *.wrp>!outputLogFile!.wlg

								REM P3D Binarization
								REM echo %binarize% -addon=%sourcePath% "-binPath=%_DAYZTOOLSPATH%\Bin\Binarize" "-textures=!targetPath!" !sourcePath! !targetPath! *.p3d !outputLogFile!.plg
								REM %binarize% -addon=%sourcePath% "-binPath=%_DAYZTOOLSPATH%\Bin\Binarize" "-textures=!targetPath!" !sourcePath! !targetPath! *.p3d>!outputLogFile!.plg

								echo %makePBO% -U -P -D "-@=!prefixName!" "!sourceFolder!" "!pboName!"
								%makePBO% -U -P -D "-@=!prefixName!" "!sourceFolder!" "!pboName!"
								%signFile% "%keyDirectory%%keyName%.biprivatekey" "!pboName!"
							)
						)
					)
				)
			)
		)
	)
)

IF exist "%workDrive%Temp\%prefixLinkRoot%\" (
	echo Removing folder "%workDrive%Temp\%prefixLinkRoot%\"
	del /F /S /Q "%workDrive%Temp\%prefixLinkRoot%\">NUL
	rmdir /S /Q "%workDrive%Temp\%prefixLinkRoot%\">NUL
)

endlocal