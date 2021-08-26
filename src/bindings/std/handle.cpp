#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/add_on/scripthandle/scripthandle.h"

static StdExtension handleExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptHandle(engine);
});
