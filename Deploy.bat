@echo off

echo Deploying to Release. Are you sure?
pause

start /W "Build PBO" "%CD%/DayZEditor/Workbench/Batchfiles/ZBinarizeDeploy.bat"

echo Uploading to Workshop
cd "%CD%/DayZEditor/Workbench/Deployment"
start /W "Upload to Workshop" Deployment.exe release