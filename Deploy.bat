@echo off

echo Deploying to Release. Are you sure?
pause

start /W "Build PBO" "%CD%/DayZEditor/Workbench/Batchfiles/ZBinarizeDeploy.bat"
start /W "Upload to Workshop" "%CD%/DayZEditor/Workbench/Deployment/Deployment.exe" release