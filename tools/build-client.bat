@echo off

pushd ..

IF NOT EXIST build\ (
    mkdir build
)
pushd build
cmake -DCLIENT_MODULE=1 ..
cmake --build . --config Release
popd

IF NOT EXIST dist\ (
    mkdir dist
    mkdir dist\client
    mkdir dist\server
)
copy build\win64\client\Release\angelscript-client-module.dll dist\client
copy build\win64\client\Release\angelscript-client-module.pdb dist\client

IF EXIST tools\extra.bat (
    tools\extra.bat
)
