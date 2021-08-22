#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scriptany/scriptany.h"

static ModuleExtension anyExtension("", [](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptAny(engine);
});
