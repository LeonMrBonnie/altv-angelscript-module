@echo off

pushd ..

IF NOT EXIST build\ (
    mkdir build
)
pushd build
cmake -DSERVER_MODULE=1 -DCLIENT_MODULE=1 ..
cmake --build . --target clangformat
popd
