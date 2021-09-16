# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("altv-angelscript-module")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DMODULE_VERSION="${MODULE_VERSION}")

if(SERVER_MODULE)
	set(PROJECT_MODULE_NAME "angelscript-module")
	set(COMPILE_FOLDER "server")
	add_definitions(-DALT_SERVER_API)
	add_definitions(-DSERVER_MODULE)
elseif(CLIENT_MODULE)
	set(PROJECT_MODULE_NAME "angelscript-client-module")
	set(COMPILE_FOLDER "client")
	add_definitions(-DALT_CLIENT_API)
	add_definitions(-DCLIENT_MODULE)
endif()

if(UNIX)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/linux64/${COMPILE_FOLDER})
	set(JIT_COMPILER_FILE ${PROJECT_SOURCE_DIR}/deps/angelscript-jit-compiler/virtual_asm_linux.cpp)
	set(ANGELSCRIPT_LIB libangelscript.a)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/win64/${COMPILE_FOLDER})
	set(JIT_COMPILER_FILE ${PROJECT_SOURCE_DIR}/deps/angelscript-jit-compiler/virtual_asm_windows.cpp)
	set(ANGELSCRIPT_LIB angelscript.lib)
	set(CRYPTOPP_LIB ${PROJECT_SOURCE_DIR}/deps/cryptopp/cryptlib.lib)
    set(PROFILER_LIB winmm.lib)
endif()
set(ANGELSCRIPT_LIB_PATH ${PROJECT_SOURCE_DIR}/deps/angelscript/lib/${ANGELSCRIPT_LIB})

# Set include path
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Download dependencies
include(DownloadDeps)
download_deps()

# Set debug config options
include(DebugConfig)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "./src/*.h" "./src/*.cpp")

include_directories(
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/deps"
    "${PROJECT_SOURCE_DIR}/deps/angelscript"
)

# Add profiler if windows
if(WIN32)
    set(PROFILER_FILE ${PROJECT_SOURCE_DIR}/deps/profiler/profiler.cpp)
endif()

add_library(${PROJECT_MODULE_NAME} SHARED
	${PROJECT_SOURCE_FILES}
	# AngelScript JIT Compiler
	${PROJECT_SOURCE_DIR}/deps/angelscript-jit-compiler/as_jit.cpp
	${PROJECT_SOURCE_DIR}/deps/angelscript-jit-compiler/virtual_asm_x64.cpp
	${JIT_COMPILER_FILE}
    # Profiler
    ${PROFILER_FILE}
)

if(WIN32)
    set_target_properties(${PROJECT_MODULE_NAME} PROPERTIES
        COMPILE_FLAGS
            "/W1 /nologo /EHsc /DEBUG"
        COMPILE_FLAGS_DEBUG
            "/MDd /Od /Ob0 /Zi"
        COMPILE_FLAGS_RELEASE
            "/MD /O2 /Ob2"
    )
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
endif()

target_link_libraries(${PROJECT_MODULE_NAME} PRIVATE
  ${ANGELSCRIPT_LIB_PATH}
  ${CRYPTOPP_LIB}
  ${PROFILER_LIB}
)
