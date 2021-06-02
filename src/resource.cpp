#pragma warning(disable : 4003)
#include "resource.h"
#include "runtime.h"
#include "helpers/module.h"
#include "helpers/events.h"
#include "angelscript/addon/scriptany/scriptany.h"
#include "helpers/convert.h"
#include "helpers/benchmark.h"
#include "helpers/angelscript.h"
#include <regex>

#include "./helpers/bytecode.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #define IS_WINDOWS
    #include "windows.h"
    #define FILE_SEPERATOR "\\"
#else
    #define IS_LINUX
    #define FILE_SEPERATOR "/"
#endif

bool AngelScriptResource::Start()
{
#ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("ResourceStart_" + resource->GetName().ToString());
#endif

    // Get main filename and extension
    std::string main      = resource->GetMain().ToString();
    std::string extension = main.substr(main.find_last_of(".") + 1);
    std::string fileName  = main.substr(0, main.find_last_of("."));

    // Load file
    auto src = ReadFile(resource->GetMain());
    if(src.IsEmpty())
    {
        Log::Error << "Failed to read resource main file" << Log::Endl;
        return false;
    }

    // Compile file
    CScriptBuilder builder;

    builder.SetIncludeCallback(Helpers::IncludeHandler, this);
    builder.SetPragmaCallback(Helpers::PragmaHandler, this);

    RegisterDefines(builder);

    int r = builder.StartNewModule(runtime->GetEngine(), resource->GetName().CStr());
    CHECK_AS_RETURN("Builder start", r, false);

    module = builder.GetModule();

    // Check if the file is precompiled binary code
    if(extension == "asb")
    {
        BytecodeStream byteStream(resource->GetPath().ToString() + FILE_SEPERATOR + main, true);
        if(byteStream.HasErrored())
        {
            Log::Error << "Failed to open bytecode file" << Log::Endl;
            return false;
        }
        r = module->LoadByteCode(&byteStream);
        CHECK_AS_RETURN("Load bytecode", r, false);
        byteStream.Close();
    }
    else
    {
        r = builder.AddSectionFromMemory(main.c_str(), src.CStr(), src.GetSize());
        CHECK_AS_RETURN("Adding section", r, false);

        r = builder.BuildModule();
        CHECK_AS_RETURN("Compilation", r, false);
    }

// Check if compile only mode
#if defined(IS_WINDOWS) && defined(SERVER_MODULE)
    std::string commandline(GetCommandLineA());
    if(commandline.find("--save-bytecode") != std::string::npos && extension != "asb")
    {
        Helpers::BytecodeStream byteStream(resource->GetPath().ToString() + FILE_SEPERATOR + fileName + ".asb");
        if(byteStream.HasErrored())
        {
            Log::Error << "Failed to open bytecode file" << Log::Endl;
            return false;
        }
        r = module->SaveByteCode(&byteStream);
        byteStream.Close();
        CHECK_AS_RETURN("Save bytecode", r, false);
        Log::Colored << "~g~Successfully saved the compiled bytecode to the file ~w~" << fileName + ".asb" << Log::Endl;

        return true;
    }
#endif

    RegisterImports();
    RegisterExports(builder);

    // Start script
    context = runtime->GetEngine()->CreateContext();
    context->SetUserData(this);

    // Register Metadata
    bool result = RegisterMetadata(builder, context);
    if(!result) return false;

    if(scriptClasses.size() != 0) return true;

    // Get the global start function if no main script class was found
    auto func = module->GetFunctionByDecl("void Start()");
    // If main function was still not found, return an error
    if(func == nullptr)
    {
        Log::Error << "The main entrypoint ('void Start()') was not found" << Log::Endl;
        module->Discard();
        context->Release();
        return false;
    }
    r = context->Prepare(func);
    CHECK_AS_RETURN("Context prepare", r, false);

    // Execute script
    r = context->Execute();
    CHECK_FUNCTION_RETURN(r, false);
    context->Unprepare();

    return true;
}

alt::String AngelScriptResource::ReadFile(alt::String path)
{
    auto pkg = resource->GetPackage();
    // Check if file exists
    if(!pkg->FileExists(path)) return alt::String();
    // Open file
    alt::IPackage::File* pkgFile = pkg->OpenFile(path);
    alt::String          src(pkg->GetFileSize(pkgFile));
    // Read file content
    pkg->ReadFile(pkgFile, src.GetData(), src.GetSize());
    pkg->CloseFile(pkgFile);

    return src;
}

bool AngelScriptResource::Stop()
{
    // Gets Stop function and if exists calls it
    if(module != nullptr)
    {
        asIScriptFunction* func = module->GetFunctionByDecl("void Stop()");
        if(func != nullptr && context != nullptr)
        {
            auto r = context->Prepare(func);
            CHECK_AS_RETURN("Stop function call", r, false);

            r = context->Execute();
            CHECK_FUNCTION_RETURN(r, true);
            context->Unprepare();
        }
        module->Discard();
    }

    // Release the event handler script functions to not create a memory leak
    for(auto pair : eventHandlers)
    {
        pair.second->Release();
    }
    eventHandlers.clear();

    for(auto kv : customLocalEventHandlers)
    {
        kv.second->Release();
    }
    customLocalEventHandlers.clear();

    for(auto kv : customRemoteEventHandlers)
    {
        kv.second->Release();
    }
    customRemoteEventHandlers.clear();

    if(scriptClasses.size() != 0)
    {
        for(auto scriptClass : scriptClasses)
        {
            auto type = scriptClass->GetObjectType();
            for(uint32_t n = 0; n < type->GetMethodCount(); n++)
            {
                auto method     = type->GetMethodByIndex(n);
                auto localEvent = method->GetUserData(1);
                if(localEvent != nullptr) delete(std::string*)localEvent;
                auto remoteEvent = method->GetUserData(2);
                if(remoteEvent != nullptr) delete(std::string*)remoteEvent;
            }
            scriptClass->Release();
        }
        scriptClasses.clear();
    }

    if(context != nullptr) context->Release();

    return true;
}

bool AngelScriptResource::OnEvent(const alt::CEvent* ev)
{
#ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("OnEvent_" + resource->GetName().ToString() + "_" + std::string(std::to_string((uint16_t)ev->GetType())));
#endif

    if(ev->GetType() == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
    {
#ifdef SERVER_MODULE
        HandleCustomEvent(ev, true);
#else
        HandleCustomEvent(ev, false);
#endif
        return true;
    }
    else if(ev->GetType() == alt::CEvent::Type::CLIENT_SCRIPT_EVENT)
    {
#ifdef SERVER_MODULE
        HandleCustomEvent(ev, false);
#else
        HandleCustomEvent(ev, true);
#endif
        return true;
    }
#ifdef CLIENT_MODULE
    else if(ev->GetType() == alt::CEvent::Type::WEB_VIEW_EVENT)
    {
        HandleWebviewEvent(static_cast<const alt::CWebViewEvent*>(ev));
        return true;
    }
    else if(ev->GetType() == alt::CEvent::Type::WEB_SOCKET_CLIENT_EVENT)
    {
        HandleWebsocketEvent(static_cast<const alt::CWebSocketClientEvent*>(ev));
        return true;
    }
#endif
    // Get the handler for the specified event
    auto event = Helpers::Event::GetEvent(ev->GetType());
    if(event == nullptr)
    {
        Log::Error << "Unhandled event type " << std::to_string((uint16_t)ev->GetType()) << Log::Endl;
        return true;
    }
    // Get all script callbacks for the event
    auto callbacks  = GetEventHandlers(ev->GetType());
    auto returnType = event->GetReturnType();
    // If the return type of the event is bool, it should return a value
    bool shouldReturn = strcmp(returnType, "bool") == 0;

    // Loop over all script callbacks and call them with the args
    for(auto callback : callbacks)
    {
        auto r = context->Prepare(callback);
        CHECK_AS_RETURN("Prepare event handler", r, true);
        // Set the event args and execute callback
        r = event->Execute(this, ev);
        CHECK_FUNCTION_RETURN(r, true);
        if(r == asEXECUTION_FINISHED && shouldReturn)
        {
            auto result = context->GetReturnByte();
            context->Unprepare();
            return result == 1 ? true : false;
        }
        context->Unprepare();
    }

    // Check if any script class exists
    if(scriptClasses.size() != 0)
    {
        for(auto scriptClass : scriptClasses)
        {
            // Get the method of the script class for the event if it exists
            auto               type      = scriptClass->GetObjectType();
            asIScriptFunction* eventFunc = nullptr;
            for(asUINT i = 0; i < type->GetMethodCount(); i++)
            {
                auto func = type->GetMethodByIndex(i, true);
                auto data = func->GetUserData();
                if(data == nullptr) continue;

                Event* eventData = static_cast<Event*>(data);
                if(eventData->GetType() != ev->GetType()) continue;

                eventFunc = func;
                break;
            }
            if(eventFunc != nullptr)
            {
                int r = context->Prepare(eventFunc);
                CHECK_AS_RETURN("Prepare script class event method", r, true);
                r = context->SetObject(scriptClass);
                CHECK_AS_RETURN("Set script class event method object", r, true);
                r = event->Execute(this, ev);
                CHECK_FUNCTION_RETURN(r, true);
                if(r == asEXECUTION_FINISHED && shouldReturn)
                {
                    auto result = context->GetReturnByte();
                    context->Unprepare();
                    return result == 1 ? true : false;
                }
                context->Unprepare();
            }
        }
    }

    return true;
}

void AngelScriptResource::HandleCustomEvent(const alt::CEvent* event, bool local)
{
#ifdef DEBUG_MODE
    Helpers::Benchmark benchmark("HandleCustomEvent_" + resource->GetName().ToString() + "_" +
                                 static_cast<const alt::CServerScriptEvent*>(event)->GetName().ToString());
#endif

    std::string     name;
    alt::MValueArgs args;

#ifdef CLIENT_MODULE
    if(local)
    {
        auto ev = static_cast<const alt::CClientScriptEvent*>(event);
        name    = ev->GetName().ToString();
        args    = ev->GetArgs();
    }
    else
    {
        auto ev = static_cast<const alt::CServerScriptEvent*>(event);
        name    = ev->GetName().ToString();
        args    = ev->GetArgs();
    }
#endif
#ifdef SERVER_MODULE
    alt::Ref<alt::IPlayer> player;
    if(local)
    {
        auto ev = static_cast<const alt::CServerScriptEvent*>(event);
        name    = ev->GetName().ToString();
        args    = ev->GetArgs();
    }
    else
    {
        auto ev = static_cast<const alt::CClientScriptEvent*>(event);
        name    = ev->GetName().ToString();
        args    = ev->GetArgs();
        player  = ev->GetTarget();
    }
#endif

    std::vector<asIScriptFunction*> handlers = GetCustomEventHandlers(name, false);
    if(handlers.size() == 0) return;

    std::vector<std::tuple<int, void*>> handlerArgs;
    for(auto arg : args)
    {
        std::tuple<int, void*> converted = Helpers::MValueToValue(runtime, arg);
        handlerArgs.push_back(converted);
    }

    for(auto handler : handlers)
    {
        auto r = context->Prepare(handler);
        CHECK_AS_RETURN("Prepare custom event handler", r, );
#ifdef SERVER_MODULE
        context->SetArgObject(0, player.Get());
#endif
        for(int i = 0; i < handlerArgs.size(); i++)
        {
            int argOffset = i;
#ifdef SERVER_MODULE
            argOffset += 1;
#endif
            auto [typeId, ptr] = handlerArgs[i];
            int ret;
            if(Helpers::IsTypePrimitive(typeId)) ret = context->SetArgAddress(argOffset, ptr);
            else
                ret = context->SetArgObject(argOffset, ptr);
            CHECK_AS_RETURN("Set custom event handler arg", ret, );
        }
        r = context->Execute();
        CHECK_FUNCTION_RETURN(r, );
    }

    // Check if any script classes exist
    if(scriptClasses.size() != 0)
    {
        for(auto scriptClass : scriptClasses)
        {
            // Get the method of the script class for the event if it exists
            auto               type      = scriptClass->GetObjectType();
            asIScriptFunction* eventFunc = nullptr;
            for(asUINT i = 0; i < type->GetMethodCount(); i++)
            {
                auto func = type->GetMethodByIndex(i, true);
                auto data = func->GetUserData(2);
                if(data == nullptr) continue;

                std::string* evName = static_cast<std::string*>(data);
                if(*evName != name) continue;

                eventFunc = func;
                break;
            }
            if(eventFunc != nullptr)
            {
                int r = context->Prepare(eventFunc);
                CHECK_AS_RETURN("Prepare script class event method", r, );
                r = context->SetObject(scriptClass);
                CHECK_AS_RETURN("Set script class event method object", r, );
#ifdef SERVER_MODULE
                context->SetArgObject(0, player.Get());
#endif
                for(int i = 0; i < handlerArgs.size(); i++)
                {
                    int argOffset = i;
#ifdef SERVER_MODULE
                    argOffset += 1;
#endif
                    auto [typeId, ptr] = handlerArgs[i];
                    int ret;
                    if(Helpers::IsTypePrimitive(typeId)) ret = context->SetArgAddress(argOffset, ptr);
                    else
                        ret = context->SetArgObject(argOffset, ptr);
                    CHECK_AS_RETURN("Set custom event handler arg", ret, );
                }
                r = context->Execute();
                CHECK_FUNCTION_RETURN(r, );
            }
        }
    }
    context->Unprepare();

    for(auto [typeId, ptr] : handlerArgs)
    {
        if(typeId != runtime->GetBaseObjectTypeId()) delete ptr;
    }
}

#ifdef CLIENT_MODULE
void AngelScriptResource::HandleWebviewEvent(const alt::CWebViewEvent* event)
{
    // todo: implement webview events
}

void AngelScriptResource::HandleWebsocketEvent(const alt::CWebSocketClientEvent* event)
{
    // todo: implement web socket client events
}
#endif

void AngelScriptResource::SetObjectData(alt::IBaseObject* object, const std::string& key, int type, void* value)
{
    auto engine = runtime->GetEngine();
    if(HasObjectData(object, key))
    {
        auto [type, value] = GetObjectData(object, key);
        engine->ReleaseScriptObject(value, engine->GetTypeInfoById(type));
    }
    auto obj                   = engine->CreateScriptObjectCopy(value, engine->GetTypeInfoById(type));
    objectData.at(object)[key] = std::make_pair(type, obj);
}

bool AngelScriptResource::HasObjectData(alt::IBaseObject* object, const std::string& key)
{
    return objectData.at(object).count(key) == 1;
}

std::pair<int, void*> AngelScriptResource::GetObjectData(alt::IBaseObject* object, const std::string& key)
{
    return objectData.at(object).at(key);
}

void AngelScriptResource::DeleteObjectData(alt::IBaseObject* object, const std::string& key)
{
    auto [type, value] = objectData.at(object).at(key);
    runtime->GetEngine()->ReleaseScriptObject(value, runtime->GetEngine()->GetTypeInfoById(type));
    objectData.at(object).erase(key);
}

void AngelScriptResource::ShowDebugInfo()
{
    Log::Colored << "*************** ~y~" << resource->GetName() << " ~w~***************" << Log::Endl;
    Log::Colored << "Script classes: ~g~" << scriptClasses.size() << Log::Endl;
    Log::Colored << "Timers: ~g~" << timers.size() << Log::Endl;
    Log::Colored << "Current timer id: ~g~" << nextTimerId << Log::Endl;
    Log::Colored << "Stored objects: ~g~" << objectData.size() << Log::Endl;
    Log::Colored << "Built-in event handlers: ~g~" << eventHandlers.size() << Log::Endl;
    Log::Colored << "Local event handlers: ~g~" << customLocalEventHandlers.size() << Log::Endl;
    Log::Colored << "Remote event handlers: ~g~" << customRemoteEventHandlers.size() << Log::Endl;
    Log::Colored << "Exports: ~g~" << resource->GetExports()->GetSize() << Log::Endl;
    Log::Colored << "Imports: ~g~" << module->GetImportedFunctionCount() << Log::Endl;
    Log::Colored << "Global vars: ~g~" << module->GetGlobalVarCount() << Log::Endl;
    Log::Colored << "Global enums: ~g~" << module->GetEnumCount() << Log::Endl;
    Log::Colored << "Global functions: ~g~" << module->GetFunctionCount() << Log::Endl;
    Log::Colored << "Global classes: ~g~" << module->GetObjectTypeCount() << Log::Endl;
    Log::Colored << "Global typedefs: ~g~" << module->GetTypedefCount() << Log::Endl;
    Log::Colored << "Current context state: ~g~" << Helpers::GetContextStateName(context->GetState()) << Log::Endl;
}

void AngelScriptResource::OnTick()
{
    // Remove all invalid timers
    for(auto& id : invalidTimers) timers.erase(id);
    invalidTimers.clear();

    // Update timers
    for(auto& timer : timers)
    {
        int64_t time = GetTime();
        if(!timer.second->Update(time)) RemoveTimer(timer.first);
    }
}

void AngelScriptResource::OnCreateBaseObject(alt::Ref<alt::IBaseObject> object)
{
    objects.insert({ object->GetType(), object });
    objectData.insert({ object, std::map<std::string, std::pair<int, void*>>() });
}

void AngelScriptResource::OnRemoveBaseObject(alt::Ref<alt::IBaseObject> object)
{
    auto range = objects.equal_range(object->GetType());
    for(auto it = range.first; it != range.second; it++)
    {
        if(it->second == object)
        {
            objects.erase(it);
            break;
        }
    }
    for(auto& [key, pair] : objectData.at(object))
    {
        if(pair.first & asTYPEID_SCRIPTOBJECT)
            runtime->GetEngine()->ReleaseScriptObject(pair.second, runtime->GetEngine()->GetTypeInfoById(pair.first));
        else
            delete pair.second;
    }
    objectData.erase(object);
}

bool AngelScriptResource::RegisterMetadata(CScriptBuilder& builder, asIScriptContext* context)
{
    std::regex customEventLocalRegex("Event\\(\"(.*)\"\\)");
#ifdef SERVER_MODULE
    std::regex customEventRemoteRegex("ClientEvent\\(\"(.*)\"\\)");
#endif
#ifdef CLIENT_MODULE
    std::regex customEventRemoteRegex("ServerEvent\\(\"(.*)\"\\)");
#endif
    uint32_t count = module->GetObjectTypeCount();
    for(uint32_t i = 0; i < count; i++)
    {
        // Get the type for the class
        auto type = module->GetObjectTypeByIndex(i);

        // Get metadata for the class
        std::vector<std::string> metadata = builder.GetMetadataForType(type->GetTypeId());
        for(auto meta : metadata)
        {
            // The metadata equals Script so its a script class
            if(meta != "Script") continue;

            // Create an instance of the script class
            auto factory = type->GetFactoryByIndex(0);
            if(factory == nullptr)
            {
                Log::Error << "Script class '" << type->GetName() << "' has no factory" << Log::Endl;
                continue;
            }
            context->Prepare(factory);
            int r = context->Execute();
            CHECK_FUNCTION_RETURN(r, false);
            asIScriptObject* obj = *(asIScriptObject**)context->GetAddressOfReturnValue();
            obj->AddRef();
            // Store our script class instance
            scriptClasses.push_back(obj);
            context->Unprepare();

            // Get all methods and check their metadata
            for(uint32_t n = 0; n < type->GetMethodCount(); n++)
            {
                auto method = type->GetMethodByIndex(n, true);
                // Get metadata for the method
                std::vector<std::string> methodMetas = builder.GetMetadataForTypeMethod(type->GetTypeId(), method);

                for(auto methodMeta : methodMetas)
                {
                    // Get the event associated with the metadata
                    Event* event = Event::GetByMetadata(methodMeta);
                    if(event != nullptr)
                    {
                        // Store the event on the method
                        method->SetUserData(event);
                        continue;
                    }
                    // Check for local custom events
                    std::smatch results;
                    auto        result = std::regex_search(methodMeta.cbegin(), methodMeta.cend(), results, customEventLocalRegex);
                    if(result)
                    {
                        auto eventName = results[1].str();
                        // Store the custom event name on the method
                        method->SetUserData(new std::string(eventName), 1);
                        continue;
                    }
                    // Check for remote custom events
                    result = std::regex_search(methodMeta.cbegin(), methodMeta.cend(), results, customEventRemoteRegex);
                    if(result)
                    {
                        auto eventName = results[1].str();
                        // Store the custom event name on the method
                        method->SetUserData(new std::string(eventName), 2);
                        continue;
                    }
                }
            }
        }
    }
    return true;
}

void AngelScriptResource::RegisterDefines(CScriptBuilder& builder)
{
    // Debug
    if(alt::ICore::Instance().IsDebug()) builder.DefineWord("DEBUG");

// Client / Server
#ifdef SERVER_MODULE
    builder.DefineWord("SERVER");
#endif
#ifdef CLIENT_MODULE
    builder.DefineWord("CLIENT");
#endif
}

void AngelScriptResource::RegisterImports()
{
    for(uint32_t i = 0; i < module->GetImportedFunctionCount(); i++)
    {
        auto source = module->GetImportedFunctionSourceModule(i);
        auto decl   = module->GetImportedFunctionDeclaration(i);

        // Get the resource and check if its started
        auto sourceResource = alt::ICore::Instance().GetResource(source);
        if(!sourceResource || !sourceResource->IsStarted())
        {
            Log::Error << "Could not bind function '" << decl << "' from module '" << source << "'" << Log::Endl;
            return;
        }

        auto exports = sourceResource->GetExports();
        // The resource is a angelscript resource
        if(sourceResource->GetType() == MODULE_TYPE)
        {
            auto sourceModule = runtime->GetEngine()->GetModule(sourceResource->GetName().CStr());
            auto func         = sourceModule->GetFunctionByDecl(decl);
            if(func == nullptr || exports->Get(func->GetName()).IsEmpty())
            {
                Log::Error << "Could not bind function '" << decl << "' from module '" << source << "'" << Log::Endl;
                return;
            }
            module->BindImportedFunction(i, func);
        }
        // The resource is not a angelscript resource
        else
        {
            Log::Warning << "Importing a function from a non-AngelScript resource is currently not possible" << Log::Endl;
            // todo: add import of functions from non angelscript resources
            /*
            for(auto it = exports->Begin(); it; it = exports->Next())
            {
                auto name = it->GetKey();
                auto func = it->GetValue().As<alt::MValueFunctionConst>();
            }*/
        }
    }
}

void AngelScriptResource::RegisterExports(CScriptBuilder& builder)
{
    alt::MValueDict exports = alt::ICore::Instance().CreateMValueDict();

    uint32_t funcs = module->GetFunctionCount();
    for(uint32_t i = 0; i < funcs; i++)
    {
        // Get the function and its metadata
        auto func = module->GetFunctionByIndex(i);
        if(func == nullptr) continue;
        auto meta = builder.GetMetadataForFunc(func);
        if(meta.size() == 0) continue;

        // Check if it has the "Export" metadata
        auto found = false;
        for(auto& metaString : meta)
        {
            if(metaString == "Export")
            {
                found = true;
                break;
            }
        }
        if(!found) continue;

        // Create and set the mvalue function
        auto mvalueFunc = new MValueFunc(this, func);
        exports->Set(func->GetName(), alt::ICore::Instance().CreateMValueFunction(mvalueFunc));
    }
    uint32_t vars = module->GetGlobalVarCount();
    for(uint32_t i = 0; i < vars; i++)
    {
        auto decl = module->GetGlobalVarDeclaration(i);
        auto meta = builder.GetMetadataForVar(i);
        if(meta.size() == 0) continue;

        // Check if it has the "Export" metadata
        auto found = false;
        for(auto& metaString : meta)
        {
            if(metaString == "Export")
            {
                found = true;
                break;
            }
        }
        if(!found) continue;

        auto parts = Helpers::SplitString(decl, " ");
        auto type  = module->GetEngine()->GetTypeIdByDecl(parts[0].c_str());
        auto name  = parts[1];
        if(type < 0)
        {
            Log::Warning << "Invalid type for exported variable " << name << ": " << type << Log::Endl;
            continue;
        }

        auto mvalue = Helpers::ValueToMValue(type, module->GetAddressOfGlobalVar(i));
        exports->Set(name, mvalue);
    }

    resource->SetExports(exports);
}
