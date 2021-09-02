#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/add_on/scriptmath/scriptmath.h"

static StdExtension mathExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptMath(engine);
});
