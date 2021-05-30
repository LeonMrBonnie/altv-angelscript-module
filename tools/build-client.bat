@echo off

:: Set version
set VERSION=DEBUG
IF NOT "%1" == "" (
    set VERSION=%1
) 

:: Build project
pushd ..
IF NOT EXIST build\ (
    mkdir build
)
pushd build
cmake -G"Visual Studio 16" -DCLIENT_MODULE=1 -DMODULE_VERSION=%VERSION% ..
cmake --build . --config Release
popd

:: Copy output files
IF NOT EXIST dist\ (
    mkdir dist
    mkdir dist\client
    mkdir dist\server
)
copy build\win64\client\Release\angelscript-client-module.dll dist\client
copy build\win64\client\Release\angelscript-client-module.pdb dist\client

:: Run post-compile script
IF EXIST tools\extra-client.bat (
    tools\extra-client.bat
)
