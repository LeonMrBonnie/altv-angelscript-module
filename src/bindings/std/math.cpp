#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scriptmath/scriptmath.h"

static StdExtension mathExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptMath(engine);
});
