#include "../../helpers/module.h"

using namespace Helpers;

static DataExtension hashExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    // Common joaat hash
    REGISTER_TYPEDEF("Hash", "uint32");
});
