#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/add_on/datetime/datetime.h"

static StdExtension datetimeExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterScriptDateTime(engine);
});
