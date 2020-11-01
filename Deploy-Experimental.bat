@echo off

echo Deploying to Experimental. Are you sure?

start /W "Build PBO" "%CD%/DayZEditor/Workbench/Batchfiles/ZBinarizeDeploy.bat"
echo Uploading to Workshop
cd "%CD%/DayZEditor/Workbench/Deployment"
start /W "Upload to Workshop" Deployment.exe experimental
