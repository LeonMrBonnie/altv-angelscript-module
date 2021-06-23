#include "../../helpers/module.h"

using namespace Helpers;

static DataExtension scriptIdExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    // Common joaat hash
    REGISTER_TYPEDEF("ScriptID", "uint32");
});
