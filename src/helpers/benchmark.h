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
    
    public:
        Benchmark(std::string name) : name(name), start(std::chrono::high_resolution_clock::now()) {}
        ~Benchmark()
        {
            End();
        }

        void End()
        {
            #ifdef DEBUG_MODE
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(3) << "~b~[BENCHMARK]~w~ " << name << ": " << (float)((float)duration / (float)1000) << "ms";
            Log::Colored << stream.str() << Log::Endl;
            #endif
        }
    };
}
