# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("altv-angelscript-module")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DAS_MIPS)
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
	set(ANGELSCRIPT_LIB ${PROJECT_SOURCE_DIR}/deps/angelscript/lib/libangelscript.a)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/win64/${COMPILE_FOLDER})
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD /Zi")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
	set(JIT_COMPILER_FILE ${PROJECT_SOURCE_DIR}/deps/angelscript-jit-compiler/virtual_asm_windows.cpp)
	set(ANGELSCRIPT_LIB ${PROJECT_SOURCE_DIR}/deps/angelscript/lib/angelscript.lib)
	set(CRYPTOPP_LIB ${PROJECT_SOURCE_DIR}/deps/cryptopp/cryptlib.lib)
endif()

file(GLOB_RECURSE PROJECT_SOURCE_FILES "./src/*.h" "./src/*.cpp")

include_directories(
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/deps"
)

add_library(${PROJECT_MODULE_NAME} SHARED
	${PROJECT_SOURCE_FILES}
	# AngelScript JIT Compiler
	${PROJECT_SOURCE_DIR}/deps/angelscript-jit-compiler/as_jit.cpp
	${PROJECT_SOURCE_DIR}/deps/angelscript-jit-compiler/virtual_asm_x64.cpp
	${JIT_COMPILER_FILE}
)

target_link_libraries(${PROJECT_MODULE_NAME} PRIVATE
  ${ANGELSCRIPT_LIB}
  ${CRYPTOPP_LIB}
)

if(TARGET clangformat)
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
    include(ClangFormat)
    target_clangformat_setup(${PROJECT_MODULE_NAME})
endif()
