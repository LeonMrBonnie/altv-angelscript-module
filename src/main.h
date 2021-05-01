#pragma once

// Uncomment to activate debug mode (shows benchmarks)
//#define DEBUG_MODE

#ifdef SERVER_MODULE
// Uncomment to generate docs files
//#define AS_GENERATE_DOCUMENTATION
#endif

#define MODULE_VERSION "1.1.3"
#define MODULE_TYPE    "as"

// SDK
#include "cpp-sdk/SDK.h"

#include "runtime.h"
