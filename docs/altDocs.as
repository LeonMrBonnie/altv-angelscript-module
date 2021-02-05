// Generated on Sat Feb  6 00:29:08 2021

namespace alt
{
    // ********** Funcdefs **********

    // Generic callback without arguments or return value
    funcdef void GenericCallback();

    // ********** Functions **********

    // Logs the specified message to the console
    void log(string msg);

    // Logs the specified message as a warning to the console
    void logWarning(string msg);

    // Logs the specified message as an error to the console
    void logError(string msg);

    // Sets a timeout
    uint setTimeout(GenericCallback@ callback, uint timeout);

    // Sets a interval
    uint setInterval(GenericCallback@ callback, uint interval);

    // Sets a next tick handler
    uint nextTick(GenericCallback@ callback);

    // Sets a every tick handler
    uint everyTick(GenericCallback@ callback);

    // Clears specified timer
    void clearTimer(uint timerId);

    // Registers a event handler
    void on(string event, GenericCallback@ callback);
}
