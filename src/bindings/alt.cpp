#include "Log.h"
#include "../helpers/module.h"
#include "../runtime.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "../helpers/convert.h"
#include "../helpers/angelscript.h"
#include "../helpers/benchmark.h"
#include "data/discord.h"

using namespace Helpers;

static void Log(const std::string& msg)
{
    Log::Colored << msg << Log::Endl;
}

static void LogWarning(const std::string& msg)
{
    Log::Warning << msg << Log::Endl;
}

static void LogError(const std::string& msg)
{
    Log::Error << msg << Log::Endl;
}

static uint32_t SetTimeout(asIScriptFunction* callback, uint32_t timeout)
{
    GET_RESOURCE();
    return resource->CreateTimer(timeout, callback, true);
}

static uint32_t SetInterval(asIScriptFunction* callback, uint32_t interval)
{
    GET_RESOURCE();
    return resource->CreateTimer(interval, callback, false);
}

static uint32_t NextTick(asIScriptFunction* callback)
{
    GET_RESOURCE();
    return resource->CreateTimer(0, callback, true);
}

static uint32_t EveryTick(asIScriptFunction* callback)
{
    GET_RESOURCE();
    return resource->CreateTimer(0, callback, false);
}

static void ClearTimer(uint32_t id)
{
    GET_RESOURCE();
    resource->RemoveTimer(id);
}

static uint32_t Hash(std::string& value)
{
    return alt::ICore::Instance().Hash(value);
}

static CScriptArray* GetAllPlayers()
{
    GET_RESOURCE();
    static asITypeInfo* playerArrayTypeInfo = nullptr;
    if(playerArrayTypeInfo == nullptr)
    {
        playerArrayTypeInfo = resource->GetRuntime()->GetEngine()->GetTypeInfoByDecl("array<alt::Player@>");
        playerArrayTypeInfo->AddRef();
    }
    auto players = alt::ICore::Instance().GetPlayers();
    auto arr     = CScriptArray::Create(playerArrayTypeInfo, players.GetSize());
    for(int i = 0; i < players.GetSize(); i++)
    {
        void* player = players[i].Get();
        arr->SetValue(i, &player);
    }
    return arr;
}

static CScriptArray* GetAllVehicles()
{
    GET_RESOURCE();
    static asITypeInfo* vehicleArrayTypeInfo = nullptr;
    if(vehicleArrayTypeInfo == nullptr)
    {
        vehicleArrayTypeInfo = resource->GetRuntime()->GetEngine()->GetTypeInfoByDecl("array<alt::Vehicle@>");
        vehicleArrayTypeInfo->AddRef();
    }
    auto vehicles = alt::ICore::Instance().GetVehicles();
    auto arr      = CScriptArray::Create(vehicleArrayTypeInfo, vehicles.GetSize());
    for(int i = 0; i < vehicles.GetSize(); i++)
    {
        void* vehicle = vehicles[i].Get();
        arr->SetValue(i, &vehicle);
    }
    return arr;
}

static CScriptArray* GetAllEntities()
{
    GET_RESOURCE();
    static asITypeInfo* entityArrayTypeInfo = nullptr;
    if(entityArrayTypeInfo == nullptr)
    {
        entityArrayTypeInfo = resource->GetRuntime()->GetEngine()->GetTypeInfoByDecl("array<alt::Entity@>");
        entityArrayTypeInfo->AddRef();
    }
    auto entities = alt::ICore::Instance().GetEntities();
    auto arr      = CScriptArray::Create(entityArrayTypeInfo, entities.GetSize());
    for(int i = 0; i < entities.GetSize(); i++)
    {
        void* entity = entities[i].Get();
        arr->SetValue(i, &entity);
    }
    return arr;
}

static std::string GetResourceName()
{
    GET_RESOURCE();
    return resource->GetIResource()->GetName().ToString();
}

static std::string GetResourceMain()
{
    GET_RESOURCE();
    return resource->GetIResource()->GetMain().ToString();
}

static std::string GetResourcePath(const std::string& name)
{
    auto resource = alt::ICore::Instance().GetResource(name);
    AS_ASSERT(resource != nullptr, "Resource not found", std::string());
    return resource->GetPath().ToString();
}

static bool HasResource(const std::string& name)
{
    auto resource = alt::ICore::Instance().GetResource(name);
    return resource != nullptr && resource->IsStarted();
}

static CScriptDictionary* GetResourceExports(const std::string& name)
{
    GET_RESOURCE();
    auto res = alt::ICore::Instance().GetResource(name);
    AS_ASSERT(res != nullptr, "Resource not found", nullptr);

    auto exports = res->GetExports();
    auto dict    = CScriptDictionary::Create(resource->GetRuntime()->GetEngine());
    for(auto it = exports->Begin(); it; it = exports->Next())
    {
        auto value = it->GetValue();
        if(value->GetType() == alt::IMValue::Type::FUNCTION) continue;
        auto [type, val] = Helpers::MValueToValue(resource->GetRuntime(), value);
        // Set the type to int64 if its an uint, because the dict doesn't support uint
        if(type == asTYPEID_UINT64) type = asTYPEID_INT64;
        dict->Set(it->GetKey().ToString(), val, type);
    }
    return dict;
}

#ifdef SERVER_MODULE
static void StartResource(const std::string& name)
{
    alt::ICore::Instance().StartResource(name);
}

static void StopResource(const std::string& name)
{
    alt::ICore::Instance().StopResource(name);
}

static void RestartResource(const std::string& name)
{
    alt::ICore::Instance().RestartResource(name);
}

static uint32_t GetNetTime()
{
    return alt::ICore::Instance().GetNetTime();
}

static void SetPassword(const std::string& password)
{
    alt::ICore::Instance().SetPassword(password);
}

static int GetDefaultDimension()
{
    return alt::DEFAULT_DIMENSION;
}

static int GetGlobalDimension()
{
    return alt::GLOBAL_DIMENSION;
}
#endif

static std::string GetVersion()
{
    return alt::ICore::Instance().GetVersion().ToString();
}

static std::string GetBranch()
{
    return alt::ICore::Instance().GetBranch().ToString();
}

static uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}

static void On(const std::string& name, const std::string& handlerName)
{
    GET_RESOURCE();
    auto handler = resource->GetModule()->GetFunctionByName(handlerName.c_str());
    AS_ASSERT(handler != nullptr, "Invalid handler function", );

    handler->AddRef();
    resource->RegisterCustomEventHandler(name, handler, true);
}

#ifdef SERVER_MODULE
static void OnClient(const std::string& name, const std::string& handlerName)
{
    GET_RESOURCE();
    auto handler = resource->GetModule()->GetFunctionByName(handlerName.c_str());
    AS_ASSERT(handler != nullptr, "Invalid handler function", );

    handler->AddRef();
    resource->RegisterCustomEventHandler(name, handler, false);
}
#endif

static void Emit(asIScriptGeneric* gen)
{
#ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("Emit");
#endif

    GET_RESOURCE();
    void*           ref    = gen->GetArgAddress(0);
    int             typeId = 0;
    std::string     event  = *static_cast<std::string*>(ref);
    alt::MValueArgs args;

    for(int i = 1; i < gen->GetArgCount(); i++)
    {
        ref    = gen->GetArgAddress(i);
        typeId = gen->GetArgTypeId(i);
        if(typeId & asTYPEID_OBJHANDLE)
        {
            // We're receiving a reference to the handle, so we need to dereference it
            ref = *(void**)ref;
            resource->GetRuntime()->GetEngine()->AddRefScriptObject(ref, resource->GetRuntime()->GetEngine()->GetTypeInfoById(typeId));
        }
        if(typeId == asTYPEID_VOID) continue;
        auto mvalue = Helpers::ValueToMValue(typeId, ref);
        args.Push(mvalue);
    }
    alt::ICore::Instance().TriggerLocalEvent(event, args);
}

template<class T>
static T* GetByID(uint16_t id)
{
    auto ent = alt::ICore::Instance().GetEntityByID(id);
    if(ent.IsEmpty()) return nullptr;
    else
        return dynamic_cast<T*>(ent.Get());
}

static bool IsDebugMode()
{
    return alt::ICore::Instance().IsDebug();
}

static bool HasMeta(const std::string& key)
{
    return alt::ICore::Instance().HasMetaData(key);
}

static bool GetMeta(const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();
    if(!HasMeta(key)) return false;

    auto mvalue      = alt::ICore::Instance().GetMetaData(key);
    auto [type, ptr] = Helpers::MValueToValue(resource->GetRuntime(), mvalue);

    auto engine = resource->GetRuntime()->GetEngine();
    Helpers::CopyAngelscriptValue(engine, ptr, type, ref, typeId);
    return true;
}

#ifdef SERVER_MODULE
static void SetMeta(const std::string& key, void* ref, int typeId)
{
    auto mvalue = Helpers::ValueToMValue(typeId, ref);
    AS_ASSERT(mvalue->GetType() != alt::IMValue::Type::NIL, "Invalid value passed", );

    alt::ICore::Instance().SetMetaData(key, mvalue);
}

static void DeleteMeta(const std::string& key)
{
    AS_ASSERT(HasMeta(key), "Specified meta key does not exist", );

    alt::ICore::Instance().DeleteMetaData(key);
}
#endif

static bool HasSyncedMeta(const std::string& key)
{
    return alt::ICore::Instance().HasSyncedMetaData(key);
}

static bool GetSyncedMeta(const std::string& key, void* ref, int typeId)
{
    GET_RESOURCE();
    if(!HasSyncedMeta(key)) return false;

    auto mvalue      = alt::ICore::Instance().GetSyncedMetaData(key);
    auto [type, ptr] = Helpers::MValueToValue(resource->GetRuntime(), mvalue);

    auto engine = resource->GetRuntime()->GetEngine();
    Helpers::CopyAngelscriptValue(engine, ptr, type, ref, typeId);
    return true;
}

#ifdef SERVER_MODULE
static void SetSyncedMeta(const std::string& key, void* ref, int typeId)
{
    auto mvalue = Helpers::ValueToMValue(typeId, ref);
    AS_ASSERT(mvalue->GetType() != alt::IMValue::Type::NIL, "Invalid value passed", );

    alt::ICore::Instance().SetSyncedMetaData(key, mvalue);
}

static void DeleteSyncedMeta(const std::string& key)
{
    AS_ASSERT(HasSyncedMeta(key), "Specified meta key does not exist", );

    alt::ICore::Instance().DeleteSyncedMetaData(key);
}
#endif

#ifdef CLIENT_MODULE
static Data::DiscordData GetDiscordData()
{
    auto state = alt::ICore::Instance().GetDiscordManager();
    return Data::DiscordData{ state->IsUserDataReady(),
                              state->GetUserID().ToString(),
                              state->GetUsername().ToString(),
                              state->GetDiscriminator().ToString(),
                              state->GetAvatar().ToString() };
}

static void SetStat(const std::string& stat, void* ref, int typeId)
{
    auto statData = alt::ICore::Instance().GetStatData(stat);
    AS_ASSERT(statData != nullptr, "Stat does not exist", );

    std::string type = statData->GetStatType();
    AS_ASSERT(type != "NONE" && type != "PROFILE_SETTING", "Stat cannot be modified", );

    if(type == "INT" || type == "TEXTLABEL")
    {
        AS_ASSERT(typeId == asTYPEID_INT32, "Invalid type, expected int32", );
        statData->SetInt32Value(*static_cast<int32_t*>(ref));
    }
    else if(type == "INT64")
    {
        AS_ASSERT(typeId == asTYPEID_INT64, "Invalid type, expected int64", );
        statData->SetInt64Value(*static_cast<int64_t*>(ref));
    }
    else if(type == "FLOAT")
    {
        AS_ASSERT(typeId == asTYPEID_FLOAT, "Invalid type, expected float", );
        statData->SetFloatValue(*static_cast<float*>(ref));
    }
    else if(type == "BOOL")
    {
        AS_ASSERT(typeId == asTYPEID_BOOL, "Invalid type, expected bool", );
        statData->SetBoolValue(*static_cast<bool*>(ref));
    }
    else if(type == "STRING")
    {
        AS_ASSERT(typeId == AngelScriptRuntime::Instance().GetStringTypeId(), "Invalid type, expected float", );
        statData->SetStringValue(static_cast<std::string*>(ref)->c_str());
    }
    else if(type == "UINT8")
    {
        AS_ASSERT(typeId == asTYPEID_UINT8, "Invalid type, expected uint8", );
        statData->SetUInt8Value(*static_cast<uint8_t*>(ref));
    }
    else if(type == "UINT16")
    {
        AS_ASSERT(typeId == asTYPEID_UINT16, "Invalid type, expected uint16", );
        statData->SetUInt16Value(*static_cast<uint16_t*>(ref));
    }
    else if(type == "UINT32")
    {
        AS_ASSERT(typeId == asTYPEID_UINT32, "Invalid type, expected uint32", );
        statData->SetUInt32Value(*static_cast<uint32_t*>(ref));
    }
    else if(type == "UINT64" || type == "POS" || type == "DATE" || type == "PACKED" || type == "USERID")
    {
        AS_ASSERT(typeId == asTYPEID_UINT64, "Invalid type, expected uint64", );
        statData->SetUInt64Value(*static_cast<uint64_t*>(ref));
    }
}
static void GetStat(const std::string& stat, void* ref, int typeId)
{
    auto statData = alt::ICore::Instance().GetStatData(stat);
    AS_ASSERT(statData != nullptr, "Stat does not exist", );

    std::string type = statData->GetStatType();
    AS_ASSERT(type != "NONE" && type != "PROFILE_SETTING", "Stat cannot be read", );

    auto engine = AngelScriptRuntime::Instance().GetEngine();
    if(type == "INT" || type == "TEXTLABEL")
    {
        AS_ASSERT(typeId == asTYPEID_INT32, "Invalid type, expected int32", );
        auto ptr = new int32_t(statData->GetInt32Value());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "INT64")
    {
        AS_ASSERT(typeId == asTYPEID_INT64, "Invalid type, expected int64", );
        auto ptr = new int64_t(statData->GetInt64Value());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "FLOAT")
    {
        AS_ASSERT(typeId == asTYPEID_FLOAT, "Invalid type, expected float", );
        auto ptr = new float(statData->GetFloatValue());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "BOOL")
    {
        AS_ASSERT(typeId == asTYPEID_BOOL, "Invalid type, expected bool", );
        auto ptr = new bool(statData->GetBoolValue());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "STRING")
    {
        AS_ASSERT(typeId == AngelScriptRuntime::Instance().GetStringTypeId(), "Invalid type, expected float", );
        auto ptr = new std::string(statData->GetStringValue());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "UINT8")
    {
        AS_ASSERT(typeId == asTYPEID_UINT8, "Invalid type, expected uint8", );
        auto ptr = new uint8_t(statData->GetUInt8Value());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "UINT16")
    {
        AS_ASSERT(typeId == asTYPEID_UINT16, "Invalid type, expected uint16", );
        auto ptr = new uint16_t(statData->GetUInt16Value());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "UINT32")
    {
        AS_ASSERT(typeId == asTYPEID_UINT32, "Invalid type, expected uint32", );
        auto ptr = new uint32_t(statData->GetUInt32Value());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
    else if(type == "UINT64" || type == "POS" || type == "DATE" || type == "PACKED" || type == "USERID")
    {
        AS_ASSERT(typeId == asTYPEID_UINT64, "Invalid type, expected uint64", );
        auto ptr = new uint64_t(statData->GetUInt64Value());
        Helpers::CopyAngelscriptValue(engine, ptr, typeId, ref, typeId);
    }
}
static void ResetStat(const std::string& stat)
{
    auto statData = alt::ICore::Instance().GetStatData(stat);
    AS_ASSERT(statData != nullptr, "Stat does not exist", );

    std::string type = statData->GetStatType();
    AS_ASSERT(type != "NONE" && type != "PROFILE_SETTING", "Stat cannot be reset", );

    statData->Reset();
}

static void TakeScreenshotCallback(asIScriptFunction* callback)
{
    GET_RESOURCE();

    auto pair = new std::pair<AngelScriptResource*, asIScriptFunction*>(resource, callback);
    alt::ICore::Instance().TakeScreenshot(
      [](alt::StringView image, const void* userData) {
          auto pair     = static_cast<const std::pair<AngelScriptResource*, asIScriptFunction*>*>(userData);
          auto context  = pair->first->GetContext();
          auto callback = pair->second;

          context->Prepare(callback);
          context->SetArgObject(0, (void*)image.CStr());
          context->Execute();
          context->Unprepare();

          callback->Release();
          delete pair;
      },
      pair);
}
static void TakeScreenshotEvent(const std::string& eventName)
{
    GET_RESOURCE();

    auto pair = new std::pair<AngelScriptResource*, std::string>(resource, eventName);
    alt::ICore::Instance().TakeScreenshot(
      [](alt::StringView image, const void* userData) {
          auto pair      = static_cast<const std::pair<AngelScriptResource*, std::string>*>(userData);
          auto context   = pair->first->GetContext();
          auto eventName = pair->second;

          alt::MValueArgs args;
          args.Push(alt::ICore::Instance().CreateMValueString(image));
          alt::ICore::Instance().TriggerLocalEvent(eventName, args);

          delete pair;
      },
      pair);
}
static void TakeScreenshotGameOnlyCallback(asIScriptFunction* callback)
{
    GET_RESOURCE();

    auto pair = new std::pair<AngelScriptResource*, asIScriptFunction*>(resource, callback);
    alt::ICore::Instance().TakeScreenshotGameOnly(
      [](alt::StringView image, const void* userData) {
          auto pair     = static_cast<const std::pair<AngelScriptResource*, asIScriptFunction*>*>(userData);
          auto context  = pair->first->GetContext();
          auto callback = pair->second;

          context->Prepare(callback);
          context->SetArgObject(0, (void*)image.CStr());
          context->Execute();
          context->Unprepare();

          callback->Release();
          delete pair;
      },
      pair);
}
static void TakeScreenshotGameOnlyEvent(const std::string& eventName)
{
    GET_RESOURCE();

    auto pair = new std::pair<AngelScriptResource*, std::string>(resource, eventName);
    alt::ICore::Instance().TakeScreenshotGameOnly(
      [](alt::StringView image, const void* userData) {
          auto pair      = static_cast<const std::pair<AngelScriptResource*, std::string>*>(userData);
          auto context   = pair->first->GetContext();
          auto eventName = pair->second;

          alt::MValueArgs args;
          args.Push(alt::ICore::Instance().CreateMValueString(image));
          alt::ICore::Instance().TriggerLocalEvent(eventName, args);

          delete pair;
      },
      pair);
}
#endif

static ModuleExtension altExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    // Generic
    REGISTER_GLOBAL_FUNC("uint Hash(const string&in value)", Hash, "Hashes the given string using the joaat algorithm");
    REGISTER_GLOBAL_FUNC("array<Player@>@ GetAllPlayers()", GetAllPlayers, "Gets all players on the server");
    REGISTER_GLOBAL_FUNC("array<Vehicle@>@ GetAllVehicles()", GetAllVehicles, "Gets all vehicles on the server");
    REGISTER_GLOBAL_FUNC("array<Entity@>@ GetAllEntities()", GetAllEntities, "Gets all entities on the server");
    REGISTER_GLOBAL_FUNC("Player@ GetPlayerByID(uint16 id)", (GetByID<alt::IPlayer>), "Gets the player with the specified ID");
    REGISTER_GLOBAL_FUNC("Vehicle@ GetVehicleByID(uint16 id)", (GetByID<alt::IVehicle>), "Gets the vehicle with the specified ID");
    REGISTER_GLOBAL_FUNC("Entity@ GetEntityByID(uint16 id)", (GetByID<alt::IEntity>), "Gets the entity with the specified ID");
#ifdef SERVER_MODULE
    REGISTER_GLOBAL_PROPERTY("int", "defaultDimension", GetDefaultDimension);
    REGISTER_GLOBAL_PROPERTY("int", "globalDimension", GetGlobalDimension);
#endif
    REGISTER_GLOBAL_PROPERTY("string", "version", GetVersion);
    REGISTER_GLOBAL_PROPERTY("string", "branch", GetBranch);
    REGISTER_GLOBAL_PROPERTY("uint", "sdkVersion", GetSDKVersion);
    REGISTER_GLOBAL_PROPERTY("bool", "debugMode", IsDebugMode);

    // Resource
    REGISTER_GLOBAL_FUNC("bool HasResource(const string&in name)", HasResource, "Returns whether the specified resource exists and is started");
#ifdef SERVER_MODULE
    REGISTER_GLOBAL_FUNC("string GetResourcePath(const string&in name)", GetResourcePath, "Gets the path to the specified resource");
    REGISTER_GLOBAL_FUNC("void StartResource(const string&in name)", StartResource, "Starts the specified resource");
    REGISTER_GLOBAL_FUNC("void StopResource(const string&in name)", StopResource, "Stops the specified resource");
    REGISTER_GLOBAL_FUNC("void RestartResource(const string&in name)", RestartResource, "Restarts the specified resource");
#endif
    REGISTER_GLOBAL_FUNC("dictionary@ GetResourceExports(const string&in name)", GetResourceExports, "Gets the exports of the specified resource");
    REGISTER_GLOBAL_PROPERTY("string", "resourceMain", GetResourceMain);
    REGISTER_GLOBAL_PROPERTY("string", "resourceName", GetResourceName);

// Server
#ifdef SERVER_MODULE
    REGISTER_GLOBAL_FUNC("uint GetNetTime()", GetNetTime, "Gets the total time the server has been running for");
    REGISTER_GLOBAL_FUNC("void SetPassword(const string&in password)", SetPassword, "Sets the current server password");
#endif

    // Logging
    REGISTER_GLOBAL_FUNC("void Log(const string&in msg)", Log, "Logs the specified message to the console");
    REGISTER_GLOBAL_FUNC("void LogWarning(const string&in msg)", LogWarning, "Logs the specified message as a warning to the console");
    REGISTER_GLOBAL_FUNC("void LogError(const string&in msg)", LogError, "Logs the specified message as an error to the console");

    // Timers
    REGISTER_FUNCDEF("void TimerCallback()", "Callback used for timers");
    REGISTER_GLOBAL_FUNC("uint SetTimeout(TimerCallback@ callback, uint timeout)", SetTimeout, "Sets a timeout");
    REGISTER_GLOBAL_FUNC("uint SetInterval(TimerCallback@ callback, uint interval)", SetInterval, "Sets a interval");
    REGISTER_GLOBAL_FUNC("uint NextTick(TimerCallback@ callback)", NextTick, "Sets a next tick handler");
    REGISTER_GLOBAL_FUNC("uint EveryTick(TimerCallback@ callback)", EveryTick, "Sets a every tick handler");
    REGISTER_GLOBAL_FUNC("void ClearTimeout(uint timerId)", ClearTimer, "Clears specified timer");
    REGISTER_GLOBAL_FUNC("void ClearInterval(uint timerId)", ClearTimer, "Clears specified timer");
    REGISTER_GLOBAL_FUNC("void ClearNextTick(uint timerId)", ClearTimer, "Clears specified timer");
    REGISTER_GLOBAL_FUNC("void ClearEveryTick(uint timerId)", ClearTimer, "Clears specified timer");
    REGISTER_GLOBAL_FUNC("void ClearTimer(uint timerId)", ClearTimer, "Clears specified timer");

    // Events
    REGISTER_GLOBAL_FUNC("void On(const string&in event, const string&in handlerName)", On, "Registers an event handler for a local custom event");
#ifdef SERVER_MODULE
    REGISTER_GLOBAL_FUNC(
      "void OnClient(const string&in event, const string&in handlerName)", OnClient, "Registers an event handler for a remote custom event");
#endif
    REGISTER_VARIADIC_FUNC("void", "Emit", "const string&in event", 32, Emit, "Emits a local event (Max 32 args)");

    // Metadata
    REGISTER_GLOBAL_FUNC("bool HasMeta(const string&in key)", HasMeta, "Returns whether the specified meta key exists");
    REGISTER_GLOBAL_FUNC("bool GetMeta(const string&in key, ?&out outValue)", GetMeta, "Sets the specified meta key to the specified value");
#ifdef SERVER_MODULE
    REGISTER_GLOBAL_FUNC("void SetMeta(const string&in key, ?&in value)", SetMeta, "Gets the value of the specified meta key");
    REGISTER_GLOBAL_FUNC("void DeleteMeta(const string&in key)", DeleteMeta, "Deletes the specified meta key");
#endif

    REGISTER_GLOBAL_FUNC("bool HasSyncedMeta(const string&in key)", HasSyncedMeta, "Returns whether the specified synced meta key exists");
    REGISTER_GLOBAL_FUNC(
      "bool GetSyncedMeta(const string&in key, ?&out outValue)", GetSyncedMeta, "Sets the specified synced meta key to the specified value");
#ifdef SERVER_MODULE
    REGISTER_GLOBAL_FUNC("void SetSyncedMeta(const string&in key, ?&in value)", SetSyncedMeta, "Gets the value of the specified synced meta key");
    REGISTER_GLOBAL_FUNC("void DeleteSyncedMeta(const string&in key)", DeleteSyncedMeta, "Deletes the specified synced meta key");
#endif

#ifdef CLIENT_MODULE
    REGISTER_ENUM("KeyState", "Keypress state used for keypress event");
    REGISTER_ENUM_VALUE("KeyState", "UP", alt::CKeyboardEvent::KeyState::UP);
    REGISTER_ENUM_VALUE("KeyState", "DOWN", alt::CKeyboardEvent::KeyState::DOWN);

    REGISTER_GLOBAL_FUNC("DiscordData GetDiscordData()", GetDiscordData, "Gets the current discord state");

    REGISTER_GLOBAL_FUNC("void SetStat(const string&in stat, ?&in value)", SetStat, "Sets the specified stat to the specified value");
    REGISTER_GLOBAL_FUNC("void GetStat(const string&in stat, ?&out value)", GetStat, "Gets the specified stat value");
    REGISTER_GLOBAL_FUNC("void ResetStat(const string&in stat)", ResetStat, "Resets the specified stat value");

    REGISTER_FUNCDEF("void ScreenshotCallback(const string&in base64Image)", "Callback used for screenshot methods");
    REGISTER_GLOBAL_FUNC("void TakeScreenshot(ScreenshotCallback callback)", TakeScreenshotCallback, "Takes a screenshot of the alt:V window");
    REGISTER_GLOBAL_FUNC("void TakeScreenshot(const string&in eventName)",
                         TakeScreenshotEvent,
                         "Takes a screenshot of the alt:V window and emits the result to the specified event");
    REGISTER_GLOBAL_FUNC(
      "void TakeScreenshotGameOnly(ScreenshotCallback callback)", TakeScreenshotGameOnlyCallback, "Takes a screenshot of the GTA V window");
    REGISTER_GLOBAL_FUNC("void TakeScreenshotGameOnly(const string&in eventName)",
                         TakeScreenshotGameOnlyEvent,
                         "Takes a screenshot of the GTA V window and emits the result to the specified event");
#endif
});
