@echo off

echo Deploying to Experimental. Are you sure?
pause

start /W "Build PBO" "%CD%/DayZEditor/Workbench/Batchfiles/ZBinarizeDeploy.bat"
start /W "Upload to Workshop" "%CD%/DayZEditor/Workbench/Deployment/Deployment.exe" experimental