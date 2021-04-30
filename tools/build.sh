cd ..

if [ ! -d "./build" ]; then
    mkdir build
fi
cd build
cmake -DSERVER_MODULE=1 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
cd ..

if [ ! -d "./dist" ]; then
    mkdir dist
fi
if [ ! -d "./dist/server" ]; then
    mkdir dist/server
fi

cp ./build/linux64/server/libangelscript-module.so dist/server
#cp build\linux64\Release\angelscript-module.pdb dist
