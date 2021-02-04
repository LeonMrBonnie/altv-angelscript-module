@echo off

IF NOT EXIST build\ (
    mkdir build
)
pushd build
cmake ..
cmake --build . --config Release
popd

IF NOT EXIST dist\ (
    mkdir dist
)
copy build\win64\Release\angelscript-module.dll dist
copy build\win64\Release\angelscript-module.pdb dist