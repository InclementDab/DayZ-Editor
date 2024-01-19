@echo off

echo Deploying to Experimental. Are you sure?

REM start /W "Build PBO" "%CD%/Editor/Workbench/Batchfiles/ZBinarizeDeploy.bat"
echo Uploading to Workshop
cd "%CD%/Editor/Workbench/Deployment"
start /W "Upload to Workshop" Deployment.exe experimental
