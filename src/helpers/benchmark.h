#pragma once
#include "../main.h"
#include "Log.h"
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iomanip>

namespace Helpers
{
    class Benchmark
    {
        std::string name;
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
        std::chrono::time_point<std::chrono::high_resolution_clock> end;
        bool showLog;
    
    public:
        Benchmark(std::string name, bool autoStart = true, bool showLog = true) : name(name), showLog(showLog) 
        {
            if(autoStart) Start();
        }
        ~Benchmark()
        {
            End();
        }

        void Start()
        {
            start = std::chrono::high_resolution_clock::now();
        }
        float End()
        {
            if(!ended)
            {
                end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                if(showLog)
                {
                    std::ostringstream stream;
                    stream << std::fixed << std::setprecision(3) << "~b~[BENCHMARK]~w~ " << name << ": " << (float)((float)duration / (float)1000) << "ms";
                    Log::Colored << stream.str() << Log::Endl;
                }
            }
            return (float)((float)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / (float)1000);
        }

        bool ended = false;
    };
}
