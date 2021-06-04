#include "keyState.h"

#ifdef CLIENT_MODULE
static DataExtension keyStateExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    using namespace Data;
    REGISTER_VALUE_CLASS("KeyState", KeyState, asOBJ_VALUE | asOBJ_POD, "The current state of a keyboard key");
    REGISTER_PROPERTY("KeyState", "bool down", KeyState, down);
    REGISTER_PROPERTY("KeyState", "bool toggled", KeyState, toggled);
});
#endif
