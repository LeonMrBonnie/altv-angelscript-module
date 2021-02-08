#include "runtime.h"
#include "resource.h"
#include "helpers/module.h"
#include "helpers/docs.h"
#include "helpers/events.h"
#include "bindings/vector3.h"
#include "angelscript/addon/scriptstdstring/scriptstdstring.h"
#include "angelscript/addon/scripthelper/scripthelper.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptmath/scriptmath.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "angelscript/addon/datetime/datetime.h"

AngelScriptRuntime::AngelScriptRuntime()
{
    using namespace Helpers;

    engine = asCreateScriptEngine();
    engine->SetMessageCallback(asFUNCTION(Helpers::MessageHandler), 0, asCALL_CDECL);

    // Optimization
    engine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);

    // Register add-ons
    RegisterStdString(engine);
    RegisterScriptArray(engine, true);
    RegisterStdStringUtils(engine);
    RegisterScriptDictionary(engine);
    RegisterScriptMath(engine);
    RegisterScriptAny(engine);
    RegisterScriptDateTime(engine);
    RegisterExceptionRoutines(engine);

    // Create docs
    Helpers::DocsGenerator altGen("alt");
    auto docs = &altGen;

    Helpers::RegisterVector3(engine, docs);

    // Register global alt functions
    REGISTER_REF_CLASS("BaseObject", alt::IBaseObject, asOBJ_REF, "Base object superclass for all alt:V base objects");
    REGISTER_REF_CLASS("WorldObject", alt::IWorldObject, asOBJ_REF, "World object superclass for all alt:V world objects");
    REGISTER_REF_CLASS("Entity", alt::IEntity, asOBJ_REF, "Entity superclass for all alt:V entities");
    REGISTER_REF_CLASS("Player", alt::IPlayer, asOBJ_REF, "alt:V Player Entity");
    ModuleExtension::RegisterAll("alt", engine, docs);

    // Register events
    Event::RegisterAll(engine, docs);

    // Generate docs
    docs->Generate();
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
