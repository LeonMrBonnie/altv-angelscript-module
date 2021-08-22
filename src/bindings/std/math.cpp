#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scriptmath/scriptmath.h"

static ModuleExtension mathExtension("", [](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptMath(engine);
});
