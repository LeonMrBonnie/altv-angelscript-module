if [ ! -d "./build" ]; then
    mkdir build
fi
cd build
cmake -DSERVER_MODULE=1 -DCLIENT_MODULE=1 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target clangformat
cd ..
