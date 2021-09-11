/*
   AngelCode Tool Box Library
   Copyright (c) 2012 Andreas Jonsson

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any
   purpose, including commercial applications, and to alter it and
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
      distribution.

   Andreas Jonsson
   andreas@angelcode.com
*/

// *** This file has been modified, and is NOT the original source code from AngelCode

//
// Define AC_PROFILE in project settings if profiling should be done
// Add a TimeIt("name the scope"); in the scope that should be profiled
// In the end the profiler will output a file profiling_summary.txt
// with the profile summary for each scope
//

// Requires winmm.lib when compiled

#pragma once

#include <string>
#include <map>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #ifdef SHOW_PROFILE
        #define AC_PROFILE
    #endif

    #ifdef AC_PROFILE
        //#define TimeIt(x) acUtility::CProfilerScope profilescope(x)
        #define TimeIt() acUtility::CProfilerScope profilescope(__FUNCTION__)
    #else
        #define TimeIt()
    #endif

namespace acUtility
{
    struct TimeCount
    {
        double time;
        int    count;
        double max;
        double min;
    };

    class CProfiler
    {
    public:
        CProfiler();
        ~CProfiler();

        double GetTime();
        double Begin(const char* name);
        void   End(const char* name, double beginTime);

    protected:
        void WriteSummary();

        double timeOffset;
        double ticksPerSecond;
        bool   usePerformance;

        typedef std::map<std::string, TimeCount> map_t;
        std::string                              currentScope;
        map_t                                    scopes;
    };

    extern CProfiler g_profiler;

    class CProfilerScope
    {
    public:
        CProfilerScope(const char* name);
        ~CProfilerScope();

    protected:
        const char* name;
        double      beginTime;
    };

}  // namespace acUtility

#else
    #define TimeIt()
#endif
