#pragma once
#include "../../helpers/benchmark.h"
#include "../../helpers/module.h"

using namespace Helpers;

static void ConstructBenchmark(void* memory, const std::string& name, bool autoStart = true, bool showLog = true)
{
    new(memory) Benchmark(name, autoStart, showLog);
}

static void DestructBenchmark(void* memory)
{
    ((Benchmark*)memory)->~Benchmark();
}

static ModuleExtension benchmarkExtension("util", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_VALUE_CLASS("Benchmark", Benchmark, asOBJ_VALUE, "Benchmarking utility");
    REGISTER_CONSTRUCTOR("Benchmark", "const string&in name = \"Unnamed_Benchmark\", bool autoStart = true, bool showLog = true", ConstructBenchmark);
    REGISTER_DESTRUCTOR("Benchmark", DestructBenchmark);
    REGISTER_METHOD("Benchmark", "void Start()", Benchmark, Start);
    REGISTER_METHOD("Benchmark", "double End()", Benchmark, End);
    REGISTER_PROPERTY("Benchmark", "bool ended", Benchmark, ended);
});
