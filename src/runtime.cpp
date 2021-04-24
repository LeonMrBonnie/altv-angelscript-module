#include "runtime.h"
#include "resource.h"
#include "helpers/module.h"
#include "helpers/events.h"
#include "helpers/angelscript.h"
#include "bindings/data/vector3.h"
#include "bindings/data/vector2.h"
#include "bindings/data/rgba.h"
#include "bindings/data/clothes.h"
#include "bindings/data/props.h"
#include "bindings/data/benchmark.h"
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

    RegisterScriptInterfaces(engine);
}

void AngelScriptRuntime::RegisterScriptInterfaces(asIScriptEngine* engine)
{
    Helpers::DocsGenerator docs("alt");
    Helpers::DocsGenerator cryptoDocs("crypto");
    Helpers::DocsGenerator utilDocs("util");
    Helpers::DocsGenerator httpDocs("http");
    Helpers::DocsGenerator jsonDocs("json");

    // Register add-ons
    RegisterScriptArray(engine, true);
    RegisterStdString(engine);
    RegisterStdStringUtils(engine);
    RegisterScriptDictionary(engine);
    RegisterScriptMath(engine);
    RegisterScriptAny(engine);
    RegisterScriptDateTime(engine);
    RegisterExceptionRoutines(engine);

    // Register classes
    Data::RegisterVector3(engine, &docs);
    Data::RegisterVector2(engine, &docs);
    Data::RegisterRGBA(engine, &docs);
#ifdef SERVER_MODULE
    Data::RegisterClothes(engine, &docs);
    Data::RegisterProps(engine, &docs);
#endif
    Data::RegisterBenchmark(engine, &docs);
    REGISTER_REF_CLASS("BaseObject", alt::IBaseObject, asOBJ_REF, "Base object superclass for all alt:V base objects");
    REGISTER_REF_CLASS("WorldObject", alt::IWorldObject, asOBJ_REF, "World object superclass for all alt:V world objects");
    REGISTER_REF_CLASS("Entity", alt::IEntity, asOBJ_REF, "Entity superclass for all alt:V entities");
    REGISTER_REF_CLASS("Player", alt::IPlayer, asOBJ_REF, "alt:V Player Entity");
    REGISTER_REF_CLASS("Vehicle", alt::IVehicle, asOBJ_REF, "alt:V Vehicle Entity");
#ifdef SERVER_MODULE
    REGISTER_REF_CLASS("VoiceChannel", alt::IVoiceChannel, asOBJ_REF, "alt:V Voice Channel");
    REGISTER_REF_CLASS("ColShape", alt::IColShape, asOBJ_REF, "alt:V Generic ColShape");
    REGISTER_REF_CLASS("ColShapeSphere", alt::IColShape, asOBJ_REF, "alt:V ColShape sphere");
    REGISTER_REF_CLASS("ColShapeCylinder", alt::IColShape, asOBJ_REF, "alt:V ColShape cylinder");
    REGISTER_REF_CLASS("ColShapeCircle", alt::IColShape, asOBJ_REF, "alt:V ColShape circle");
    REGISTER_REF_CLASS("ColShapeCuboid", alt::IColShape, asOBJ_REF, "alt:V ColShape cuboid");
    REGISTER_REF_CLASS("ColShapeRect", alt::IColShape, asOBJ_REF, "alt:V ColShape rectangle");
#endif
    REGISTER_REF_CLASS("Checkpoint", alt::IColShape, asOBJ_REF, "alt:V ColShape checkpoint");
    REGISTER_REF_CLASS("Blip", alt::IBlip, asOBJ_REF, "alt:V Blip");

    // Performance
    // todo: make template specializations work
    // Helpers::RegisterArrayTemplateSpecializations(engine);

    // Register extensions
    ModuleExtension::RegisterAll("alt", engine, &docs);
    ModuleExtension::RegisterAll("crypto", engine, &cryptoDocs);
    ModuleExtension::RegisterAll("util", engine, &utilDocs);
    ModuleExtension::RegisterAll("http", engine, &httpDocs);
    ModuleExtension::RegisterAll("json", engine, &jsonDocs);

    // Register events
    Event::RegisterAll(engine, &docs);

    // Cache type infos
    RegisterTypeInfos();

    // Generate docs
    docs.Generate();
    cryptoDocs.Generate();
    utilDocs.Generate();
    httpDocs.Generate();
    jsonDocs.Generate();
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
