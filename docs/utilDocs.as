// Generated on Fri Jul 16 02:40:48 2021

namespace util
{
    // ********** Functions **********

    // Gets the current timestamp
    uint64 GetTimestamp();

    // Prints the current callstack for debugging
    void ShowCallstack(uint maxLevels = 0);

    // Evals the given code
    bool Eval(const string&in code);

    // ********** Objects **********

    // Benchmarking utility
    class Benchmark
    {
        bool ended;

        Benchmark(const string&in name = "Unnamed_Benchmark", bool autoStart = true, bool showLog = true);
        void Start();
        double End();
    };
}
