@echo off

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
copy build\win64\client\Release\angelscript-module.dll dist\client
copy build\win64\client\Release\angelscript-module.pdb dist\client

IF EXIST extra.bat (
    extra.bat
)