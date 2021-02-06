#include "runtime.h"
#include "resource.h"
#include "helpers/module.h"
#include "helpers/docs.h"
#include "helpers/events.h"
#include "angelscript/addon/scriptstdstring/scriptstdstring.h"
#include "angelscript/addon/scripthelper/scripthelper.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptmath/scriptmath.h"
#include "angelscript/addon/scriptany/scriptany.h"

AngelScriptRuntime::AngelScriptRuntime()
{
    using namespace Helpers;

    engine = asCreateScriptEngine();
    engine->SetMessageCallback(asFUNCTION(Helpers::MessageHandler), 0, asCALL_CDECL);

    // Register add-ons
    RegisterStdString(engine);
    RegisterStdStringUtils(engine);
    RegisterScriptArray(engine, true);
    RegisterScriptDictionary(engine);
    RegisterScriptMath(engine);
    RegisterScriptAny(engine);

    // Create docs
    Helpers::DocsGenerator altGen("alt");

    // Register global alt functions
    ModuleExtension::RegisterAll("alt", engine, &altGen);

    // Register events
    Event::RegisterAll(engine, &altGen);

    // Generate docs
    altGen.Generate();
}

alt::IResource::Impl* AngelScriptRuntime::CreateImpl(alt::IResource* impl)
{
    auto resource = new AngelScriptResource(this, impl);
    return resource;
}

void AngelScriptRuntime::DestroyImpl(alt::IResource::Impl* impl)
{
    AngelScriptRuntime* resource = dynamic_cast<AngelScriptRuntime*>(impl);
    if(resource != nullptr) 
    {
        delete resource;
    }
}
