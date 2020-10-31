@echo off

echo Deploying to Experimental. Are you sure?
pause

start /W "Build PBO" "%CD%/DayZEditor/Workbench/Batchfiles/ZBinarizeDeploy.bat"
echo Uploading to Workshop
start /W "Upload to Workshop" "%CD%/DayZEditor/Workbench/Deployment/Deployment.exe" experimental