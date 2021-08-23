#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scripthandle/scripthandle.h"

static StdExtension handleExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptHandle(engine);
});
