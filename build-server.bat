@echo off

IF NOT EXIST build\ (
    mkdir build
)
pushd build
cmake -DSERVER_MODULE=1 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
popd

IF NOT EXIST dist\ (
    mkdir dist
    mkdir dist\client
    mkdir dist\server
)
copy build\win64\server\Release\angelscript-module.dll dist\server
copy build\win64\server\Release\angelscript-module.pdb dist\server

IF EXIST extra.bat (
    extra.bat
)