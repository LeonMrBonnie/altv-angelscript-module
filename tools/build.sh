# Set version
VERSION=DEBUG

if [ $# -eq 1 ]; then
    VERSION=$1
fi

# Build project
cd ..
if [ ! -d "./build" ]; then
    mkdir build
fi
cd build
cmake -DSERVER_MODULE=1 -DCMAKE_BUILD_TYPE=Release -DMODULE_VERSION=$VERSION ..
cmake --build . --config Release
cd ..

# Copy output files
if [ ! -d "./dist" ]; then
    mkdir dist
fi
if [ ! -d "./dist/server" ]; then
    mkdir dist/server
fi

cp ./build/linux64/server/libangelscript-module.so dist/server
#cp build\linux64\Release\angelscript-module.pdb dist
