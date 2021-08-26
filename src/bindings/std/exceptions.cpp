#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/add_on/scripthelper/scripthelper.h"

static StdExtension exceptionExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterExceptionRoutines(engine);
});
