#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

namespace Data
{
    struct KeyState
    {
        bool down;
        bool toggled;
    };

    static void RegisterKeyState(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("KeyState", KeyState, asOBJ_VALUE | asOBJ_POD, "The current state of a keyboard key");
        REGISTER_PROPERTY("KeyState", "bool down", KeyState, down);
        REGISTER_PROPERTY("KeyState", "bool toggled", KeyState, toggled);
    }
}  // namespace Data
