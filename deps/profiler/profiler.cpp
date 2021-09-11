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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <iostream>

#include "profiler.h"

using namespace std;

namespace acUtility
{
    CProfiler g_profiler;

    CProfiler::CProfiler()
    {
        // We need to know how often the clock is updated
        __int64 tps;
        if(!QueryPerformanceFrequency((LARGE_INTEGER*)&tps)) usePerformance = false;
        else
        {
            usePerformance = true;
            ticksPerSecond = double(tps);
        }

        timeOffset = GetTime();
    }

    CProfiler::~CProfiler()
    {
#ifdef AC_PROFILE
        WriteSummary();
#endif
    }

    double CProfiler::GetTime()
    {
        if(usePerformance)
        {
            __int64 ticks;
            QueryPerformanceCounter((LARGE_INTEGER*)&ticks);

            return double(ticks) / ticksPerSecond - timeOffset;
        }

        return double(timeGetTime()) / 1000.0 - timeOffset;
    }

    double CProfiler::Begin(const char* name)
    {
        double time = GetTime();

        // Add the scope to the key
        if(currentScope.length()) currentScope += "|";
        currentScope += name;

        // Compensate for the time spent writing to the file
        timeOffset += GetTime() - time;

        return time;
    }

    void CProfiler::End(const char* name, double beginTime)
    {
        double time = GetTime();

        double elapsed = time - beginTime;

        // Update the profile info for this scope
        map_t::iterator it = scopes.find(currentScope);
        if(it != scopes.end())
        {
            it->second.time += elapsed;
            it->second.count++;
            if(it->second.max < elapsed) it->second.max = elapsed;
            if(it->second.min > elapsed) it->second.min = elapsed;
        }
        else
        {
            TimeCount tc = { elapsed, 1, elapsed, elapsed };
            scopes.insert(map_t::value_type(currentScope, tc));
        }

        // Remove the inner most scope from the key
        int n = currentScope.rfind("|");
        if(n > 0) currentScope.resize(n);
        else
            currentScope.resize(0);

        // Compensate for the time spent writing to the file
        timeOffset += GetTime() - time;
    }

    void CProfiler::WriteSummary()
    {
        // Write the analyzed info into a file for inspection
        FILE* fp;
#if _MSC_VER >= 1500
        fopen_s(&fp, "profiling_summary.txt", "wt");
#else
        fp = fopen("profiling_summary.txt", "wt");
#endif
        if(fp == 0) return;

        fprintf(fp, "%-60s %10s %15s %15s %15s %15s\n\n", "Scope", "Count", "Tot time", "Avg time", "Max time", "Min time");

        map_t::iterator it;
        for(it = scopes.begin(); it != scopes.end(); it++)
        {
            string scope = it->first;
            size_t count = 0;
            size_t last  = -1;
            for(size_t n = scope.find("|"); n != scope.npos; n = scope.find("|", n + 1))
            {
                last = n;
                count++;
            }

            if(count)
            {
                string indent("                                               ");
                indent.resize(count);
                scope = indent + scope.substr(last + 1, string::npos);
            }
            fprintf(fp,
                    "%-60s %10d %15.6f %15.6f %15.6f %15.6f\n",
                    scope.c_str(),
                    it->second.count,
                    it->second.time,
                    it->second.time / it->second.count,
                    it->second.max,
                    it->second.min);
        }

        fclose(fp);

        std::cout << "Wrote profile to file profiling_summary.txt" << std::endl;
    }

    CProfilerScope::CProfilerScope(const char* name)
    {
        this->name = name;
        beginTime  = g_profiler.Begin(name);
    }

    CProfilerScope::~CProfilerScope()
    {
        g_profiler.End(name, beginTime);
    }

}  // namespace acUtility
