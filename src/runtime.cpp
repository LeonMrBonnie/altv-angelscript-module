#include "runtime.h"
#include "resource.h"
#include "helpers/module.h"
#include "helpers/events.h"
#include "helpers/angelscript.h"
#include "bindings/data/vector3.h"
#include "bindings/data/vector2.h"
#include "bindings/data/clothes.h"
#include "bindings/data/props.h"
#include "bindings/data/benchmark.h"
#include "bindings/data/discord.h"
#include "bindings/data/keyState.h"
#include "angelscript/addon/scriptstdstring/scriptstdstring.h"
#include "angelscript/addon/scripthelper/scripthelper.h"
#include "angelscript/addon/scriptarray/scriptarray.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptmath/scriptmath.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "angelscript/addon/datetime/datetime.h"
#include "angelscript-jit-compiler/as_jit.h"

AngelScriptRuntime::AngelScriptRuntime()
{
    using namespace Helpers;

    // Create a new AngelScript engine
    engine = asCreateScriptEngine();
    engine->SetMessageCallback(asFUNCTION(Helpers::MessageHandler), 0, asCALL_CDECL);

    // JIT compiler
    engine->SetEngineProperty(asEP_INCLUDE_JIT_INSTRUCTIONS, true);

    asCJITCompiler* jitCompiler = new asCJITCompiler(JIT_SYSCALL_FPU_NORESET | JIT_ALLOC_SIMPLE | JIT_NO_SUSPEND | JIT_FAST_REFCOUNT);
    engine->SetJITCompiler(jitCompiler);

    // Optimization
    engine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);

    if(alt::ICore::Instance().IsDebug()) engine->SetCircularRefDetectedCallback(CircularRefDetectedHandler);

    RegisterScriptInterfaces(engine);
}

void AngelScriptRuntime::RegisterScriptInterfaces(asIScriptEngine* engine)
{
#ifdef SERVER_MODULE
    Helpers::DocsGenerator docs("altServer");
#endif
#ifdef CLIENT_MODULE
    Helpers::DocsGenerator docs("altClient");
#endif

    // Register add-ons
    RegisterScriptArray(engine, true);
    RegisterStdString(engine);
    RegisterStdStringUtils(engine);
    RegisterScriptDictionary(engine);
    RegisterScriptMath(engine);
    RegisterScriptAny(engine);
    RegisterScriptDateTime(engine);
    RegisterExceptionRoutines(engine);

    // Register data
    Helpers::ModuleExtension::RegisterAllData(engine, &docs);

    // Register ref classes
    REGISTER_REF_CLASS("BaseObject", alt::IBaseObject, asOBJ_REF, "Base object superclass for all alt:V base objects", "");
    REGISTER_REF_CLASS("WorldObject", alt::IWorldObject, asOBJ_REF, "World object superclass for all alt:V world objects", "BaseObject");
    REGISTER_REF_CLASS("Entity", alt::IEntity, asOBJ_REF, "Entity superclass for all alt:V entities", "WorldObject");
    REGISTER_REF_CLASS("Player", alt::IPlayer, asOBJ_REF, "alt:V Player Entity", "Entity");
    REGISTER_REF_CLASS("Vehicle", alt::IVehicle, asOBJ_REF, "alt:V Vehicle Entity", "Entity");
#ifdef SERVER_MODULE
    REGISTER_REF_CLASS("VoiceChannel", alt::IVoiceChannel, asOBJ_REF, "alt:V Voice Channel", "BaseObject");
    REGISTER_REF_CLASS("ColShape", alt::IColShape, asOBJ_REF, "alt:V Generic ColShape", "WorldObject");
    REGISTER_REF_CLASS("ColShapeSphere", alt::IColShape, asOBJ_REF, "alt:V ColShape sphere", "ColShape");
    REGISTER_REF_CLASS("ColShapeCylinder", alt::IColShape, asOBJ_REF, "alt:V ColShape cylinder", "ColShape");
    REGISTER_REF_CLASS("ColShapeCircle", alt::IColShape, asOBJ_REF, "alt:V ColShape circle", "ColShape");
    REGISTER_REF_CLASS("ColShapeCuboid", alt::IColShape, asOBJ_REF, "alt:V ColShape cuboid", "ColShape");
    REGISTER_REF_CLASS("ColShapeRect", alt::IColShape, asOBJ_REF, "alt:V ColShape rectangle", "ColShape");
    REGISTER_REF_CLASS("Checkpoint", alt::IColShape, asOBJ_REF, "alt:V ColShape checkpoint", "ColShape");
#endif
    REGISTER_REF_CLASS("Blip", alt::IBlip, asOBJ_REF, "alt:V Blip", "WorldObject");

    // Register extensions
    ModuleExtension::RegisterAll("alt", engine, &docs);
    ModuleExtension::RegisterAll("crypto", engine);
    ModuleExtension::RegisterAll("util", engine);
    ModuleExtension::RegisterAll("http", engine);
    ModuleExtension::RegisterAll("json", engine);
    ModuleExtension::RegisterAll("file", engine);
    ModuleExtension::RegisterAll("natives", engine);

    // Set default namespace back to alt namespace
    engine->SetDefaultNamespace("alt");

    // Register events
    Event::RegisterAll(engine, &docs);

    // Cache type infos
    RegisterTypeInfos();
}

void AngelScriptRuntime::ShowDebugInfo()
{
    Log::Colored << "*************** ~y~AngelScript ~w~***************" << Log::Endl;
    Log::Colored << "Registered enums: ~g~" << engine->GetEnumCount() << Log::Endl;
    Log::Colored << "Registered funcdefs: ~g~" << engine->GetFuncdefCount() << Log::Endl;
    Log::Colored << "Registered functions: ~g~" << engine->GetGlobalFunctionCount() << Log::Endl;
    Log::Colored << "Registered variables: ~g~" << engine->GetGlobalPropertyCount() << Log::Endl;
    Log::Colored << "Registered classes: ~g~" << engine->GetObjectTypeCount() << Log::Endl;
    auto classesStats = Helpers::GetRegisteredClassesStats(engine);
    Log::Colored << "Registered class methods: ~g~" << classesStats.first << Log::Endl;
    Log::Colored << "Registered class properties: ~g~" << classesStats.second << Log::Endl;
    Log::Colored << "Registered typedefs: ~g~" << engine->GetTypedefCount() << Log::Endl;
    Log::Colored << "Registered module extensions: ~g~" << ModuleExtension::GetCount() << Log::Endl;
    Log::Colored << "Running modules: ~g~" << engine->GetModuleCount() << Log::Endl;

    asUINT curSize, totalDestroyed, totalDetected, newObjs, totalNewDestroyed;
    engine->GetGCStatistics(&curSize, &totalDestroyed, &totalDetected, &newObjs, &totalNewDestroyed);
    Log::Colored << "*** GC Stats ***" << Log::Endl;
    Log::Colored << "Current size: ~g~" << curSize << Log::Endl;
    Log::Colored << "Total destroyed: ~g~" << totalDestroyed << Log::Endl;
    Log::Colored << "Total detected: ~g~" << totalDetected << Log::Endl;
    Log::Colored << "Total new destroyed: ~g~" << totalNewDestroyed << Log::Endl;
    Log::Colored << "New objects: ~g~" << newObjs << Log::Endl;
}

void AngelScriptRuntime::RegisterTypeInfos()
{
    // Register all commonly used types once to save performance
    arrayStringTypeInfo = engine->GetTypeInfoByDecl("array<string>");
    arrayStringTypeInfo->AddRef();
    arrayIntTypeInfo = engine->GetTypeInfoByDecl("array<int>");
    arrayIntTypeInfo->AddRef();
    arrayUintTypeInfo = engine->GetTypeInfoByDecl("array<uint>");
    arrayUintTypeInfo->AddRef();
    arrayByteTypeInfo = engine->GetTypeInfoByDecl("array<uint8>");
    arrayByteTypeInfo->AddRef();
}

// Creates an array of strings
CScriptArray* AngelScriptRuntime::CreateStringArray(uint32_t len)
{
    auto arr = CScriptArray::Create(arrayStringTypeInfo, len);
    return arr;
}

// Creates an array of ints
CScriptArray* AngelScriptRuntime::CreateIntArray(uint32_t len)
{
    auto arr = CScriptArray::Create(arrayIntTypeInfo, len);
    return arr;
}

// Creates an array of unsigned ints
CScriptArray* AngelScriptRuntime::CreateUIntArray(uint32_t len)
{
    auto arr = CScriptArray::Create(arrayUintTypeInfo, len);
    return arr;
}

CScriptArray* AngelScriptRuntime::CreateByteArray(uint8_t* data)
{
    auto arr = CScriptArray::Create(arrayByteTypeInfo, data);
    return arr;
}

alt::IResource::Impl* AngelScriptRuntime::CreateImpl(alt::IResource* impl)
{
    auto resource = new AngelScriptResource(this, impl);
    resources.insert({ impl, resource });
    return resource;
}

void AngelScriptRuntime::DestroyImpl(alt::IResource::Impl* impl)
{
    AngelScriptResource* resource = dynamic_cast<AngelScriptResource*>(impl);
    if(resource != nullptr)
    {
        resources.erase(resource->GetIResource());
        delete resource;
    }
}

void AngelScriptRuntime::OnTick()
{
    asUINT curSize, totalDestroyed, totalDetected, newObjs, totalNewDestroyed;
    engine->GetGCStatistics(&curSize, &totalDestroyed, &totalDetected, &newObjs, &totalNewDestroyed);
    if(curSize > 100 && newObjs > 10)
    {
        Log::Warning << "A lot of objects detected by the garbage collector" << Log::Endl;
    }
    asUINT steps = 1;
    if(newObjs != 0) steps = (asUINT)std::ceil(newObjs % 10);

    engine->GarbageCollect(asGC_ONE_STEP, steps);
}
