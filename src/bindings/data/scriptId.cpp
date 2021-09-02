#include "../../helpers/module.h"

using namespace Helpers;

#ifdef CLIENT_MODULE

static DataExtension scriptIdExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    // ScriptIDs used in natives
    REGISTER_TYPEDEF("ScriptID", "int32");
});

#endif
