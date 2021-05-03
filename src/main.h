#pragma once

// Uncomment to activate debug mode (shows benchmarks)
//#define DEBUG_MODE

#ifdef SERVER_MODULE
// Uncomment to generate docs files
//#define AS_GENERATE_DOCUMENTATION
#endif

#ifndef MODULE_VERSION
    #define MODULE_VERSION "DEBUG"
#endif
#define MODULE_TYPE "as"

// SDK
#include "cpp-sdk/SDK.h"

#include "runtime.h"
