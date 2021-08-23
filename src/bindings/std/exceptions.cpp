#include "Log.h"
#include "helpers/module.h"
#include "helpers/angelscript.h"
#include "angelscript/addon/scripthelper/scripthelper.h"

static StdExtension exceptionExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    //
    RegisterExceptionRoutines(engine);
});
