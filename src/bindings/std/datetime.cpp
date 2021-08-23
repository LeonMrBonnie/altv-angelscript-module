#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/datetime/datetime.h"

static StdExtension datetimeExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptDateTime(engine);
});
