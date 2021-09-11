#pragma warning(disable : 4003)
#include "resource.h"
#include "runtime.h"
#include "helpers/module.h"
#include "helpers/events.h"
#include "helpers/convert.h"
#include "helpers/benchmark.h"
#include "helpers/angelscript.h"
#include "helpers/libImport.h"
#include "angelscript/add_on/scriptany/scriptany.h"
#include "profiler/profiler.h"
#include <regex>
#include <string>

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
    TimeIt();

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

    runtime->GetEngine()->BeginConfigGroup(resource->GetName().CStr());

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
            runtime->GetEngine()->EndConfigGroup();
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
        runtime->GetEngine()->EndConfigGroup();
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
    if(!result)
    {
        runtime->GetEngine()->EndConfigGroup();
        return false;
    }

    if(scriptClasses.size() != 0) return true;

    // Get the global start function if no main script class was found
    auto func = module->GetFunctionByDecl("void Start()");
    // If main function was still not found, return an error
    if(func == nullptr)
    {
        Log::Error << "The main entrypoint ('void Start()') was not found" << Log::Endl;
        module->Discard();
        context->Release();
        runtime->GetEngine()->EndConfigGroup();
        return false;
    }

    // Execute script
    CallFunction(context, func);

    runtime->GetEngine()->EndConfigGroup();

    return true;
}

alt::String AngelScriptResource::ReadFile(alt::String path) const
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

void AngelScriptResource::RegisterEventHandler(alt::CEvent::Type event, asIScriptFunction* handler)
{
    if(!Helpers::CheckEventFunctionParams(handler)) return;
    eventHandlers.insert({ event, handler });
}

bool AngelScriptResource::Stop()
{
    TimeIt();

    // Gets Stop function and if exists calls it
    if(module != nullptr)
    {
        asIScriptFunction* func = module->GetFunctionByDecl("void Stop()");
        if(func != nullptr && context != nullptr)
        {
            CallFunction(context, func);
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

    for(auto func : importedFunctions)
    {
        func->Release();
    }
    importedFunctions.clear();

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

    for(auto timer : timers)
    {
        delete timer.second;
    }
    timers.clear();

    for(auto func : libraryImportFunctions)
    {
        func->Release();
    }
    libraryImportFunctions.clear();

    for(auto dll : importedLibraries)
    {
        LibraryImport::FreeLibrary(dll);
    }
    importedLibraries.clear();

    // todo: fix cleaning up the library import functions
    // Remove the config group to clear library import functions from global namespace
    int result = runtime->GetEngine()->RemoveConfigGroup(resource->GetName().CStr());
    /*
    if(result == asCONFIG_GROUP_IS_IN_USE)
    {
        // Try to do one full cycle of the garbage collector
        runtime->GetEngine()->GarbageCollect(asGC_FULL_CYCLE);
        result = runtime->GetEngine()->RemoveConfigGroup(resource->GetName().CStr());
        if(result == asCONFIG_GROUP_IS_IN_USE)
        {
            // A dll function is still used after doing a garbage collector cycle, so it is used elsewhere
            Log::Error << "The dll import functions could not be cleaned up because they are still used somewhere" << Log::Endl;
            Log::Error << "Make sure you don't use dll import functions in other resources other than the one that registered it!" << Log::Endl;
        }
    }
    */
    return true;
}

static inline bool IsEventRemoteScriptEvent(alt::CEvent::Type type)
{
#ifdef SERVER_MODULE
    if(type == alt::CEvent::Type::CLIENT_SCRIPT_EVENT) return true;
#endif
#ifdef CLIENT_MODULE
    if(type == alt::CEvent::Type::SERVER_SCRIPT_EVENT) return true;
#endif
    return false;
}

static inline bool IsEventLocalScriptEvent(alt::CEvent::Type type)
{
#ifdef SERVER_MODULE
    if(type == alt::CEvent::Type::SERVER_SCRIPT_EVENT) return true;
#endif
#ifdef CLIENT_MODULE
    if(type == alt::CEvent::Type::CLIENT_SCRIPT_EVENT) return true;
#endif
    return false;
}

bool AngelScriptResource::OnEvent(const alt::CEvent* ev)
{
    TimeIt();

    // Custom script events
    if(IsEventLocalScriptEvent(ev->GetType()))
    {
        HandleCustomEvent(ev, true);
        return true;
    }
    else if(IsEventRemoteScriptEvent(ev->GetType()))
    {
        HandleCustomEvent(ev, false);
        return true;
    }

#ifdef CLIENT_MODULE
    // Entity events
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

    // Invoke global event handlers
    auto callbacks = GetEventHandlers(ev->GetType());
    bool cancel    = event->InvokeEventHandlers(this, ev, callbacks);

    // Check if any script class exists
    if(scriptClasses.size() != 0)
    {
        for(auto scriptClass : scriptClasses)
        {
            // Get the method of the script class for the event if it exists
            asITypeInfo*                    type = scriptClass->GetObjectType();
            std::vector<asIScriptFunction*> eventFuncs;
            for(asUINT i = 0; i < type->GetMethodCount(); i++)
            {
                auto func = type->GetMethodByIndex(i, true);
                auto data = func->GetUserData();
                if(data == nullptr) continue;

                Event* eventData = static_cast<Event*>(data);
                if(eventData->GetType() != ev->GetType()) continue;
                eventFuncs.push_back(func);
            }
            // Invoke event handlers for the script class
            if(eventFuncs.size() != 0)
            {
                bool result = event->InvokeEventHandlers(this, ev, eventFuncs, scriptClass);
                if(!cancel && result) cancel = true;
            }
        }
    }

    if(cancel) ev->Cancel();

    return true;
}

// todo: rework this shit code
void AngelScriptResource::HandleCustomEvent(const alt::CEvent* event, bool local)
{
    TimeIt();

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
    if(handlers.size() == 0 && scriptClasses.size() == 0) return;

    std::vector<std::pair<void*, int>> handlerArgs;
#ifdef SERVER_MODULE
    if(!local) handlerArgs.push_back({ player.Get(), -1 });
#endif
    for(auto arg : args)
    {
        std::tuple<int, void*> result = Helpers::MValueToValue(runtime, arg);
        auto [type, ptr]              = result;
        handlerArgs.push_back({ ptr, type });
    }

    for(auto handler : handlers)
    {
        CallFunction(context, handler, handlerArgs);
    }

    // Check if any script classes exist
    if(scriptClasses.size() != 0)
    {
        for(auto scriptClass : scriptClasses)
        {
            // Get the method of the script class for the event if it exists
            auto               type      = scriptClass->GetObjectType();
            asIScriptFunction* eventFunc = nullptr;
            bool               isGeneric = false;
            for(asUINT i = 0; i < type->GetMethodCount(); i++)
            {
                auto  func = type->GetMethodByIndex(i, true);
                void* data;
                if(local) data = func->GetUserData(1);
                else
                    data = func->GetUserData(2);
                if(data == nullptr) continue;

                std::string* evName = static_cast<std::string*>(data);
                if(*evName != name && *evName != "*") continue;

                eventFunc = func;
                if(*evName == "*") isGeneric = true;
                break;
            }
            if(eventFunc != nullptr)
            {
                std::vector<std::pair<void*, int>> args = handlerArgs;
#ifdef SERVER_MODULE
                if(!local)
                {
                    args.insert(args.begin(), { player.Get(), -1 });
                }
#endif
                if(isGeneric)
                {
                    args.insert(args.begin(), { (void*)name.c_str(), -1 });
                }
                CallFunction(context, eventFunc, handlerArgs, scriptClass);
            }
        }
    }

    for(auto [ptr, typeId] : handlerArgs)
    {
        if(typeId != -1 && typeId != runtime->GetTypeInfoCache().Get("alt::BaseObject@")->GetTypeId()) delete ptr;
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

bool AngelScriptResource::Eval(const std::string& code)
{
    std::stringstream evalCode;
    evalCode << "void exec() {" << code << " }";

    asIScriptFunction* func = nullptr;
    int                r    = module->CompileFunction("eval", evalCode.str().c_str(), 0, 0, &func);
    CHECK_AS_RETURN("Compile eval code", r, false);

    evalFunctions.push_back(func);
    return true;
}

void AngelScriptResource::ShowDebugInfo() const
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
    TimeIt();

    // Execute eval functions
    for(auto func : evalFunctions)
    {
        CallFunction(context, func);
    }
    evalFunctions.clear();

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
    objectData.insert({ object, std::unordered_map<std::string, std::pair<int, void*>>() });
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

    if(objectData.count(object) != 0)
    {
        auto& data = objectData.at(object);
        for(auto& [key, pair] : data)
        {
            if(pair.first & asTYPEID_SCRIPTOBJECT)
                runtime->GetEngine()->ReleaseScriptObject(pair.second, runtime->GetEngine()->GetTypeInfoById(pair.first));
            else
                delete pair.second;
        }
        objectData.erase(object);
    }
}

bool AngelScriptResource::RegisterMetadata(CScriptBuilder& builder, asIScriptContext* context)
{
    TimeIt();

    std::regex customEventLocalRegex("Event\\(\"(.*)\"\\)");
#ifdef SERVER_MODULE
    std::regex customEventRemoteRegex("ClientEvent\\(\"(.*)\"\\)");
#endif
#ifdef CLIENT_MODULE
    std::regex customEventRemoteRegex("ServerEvent\\(\"(.*)\"\\)");
#endif
    std::regex intervalRegex("Interval\\((.*)\\)");

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

            void* result = CallFunction(context, factory);
            if(!result) continue;
            asIScriptObject* obj = (asIScriptObject*)result;
            // Store our script class instance
            scriptClasses.push_back(obj);
            context->Unprepare();

            // Get all methods and check their metadata
            for(uint32_t n = 0; n < type->GetMethodCount(); n++)
            {
                auto method = type->GetMethodByIndex(n, true);
                // Get metadata for the method
                std::vector<std::string> methodMetas = builder.GetMetadataForTypeMethod(type->GetTypeId(), method);

                // todo: store this data on the resource instead of on the class method
                for(auto methodMeta : methodMetas)
                {
                    // Get the event associated with the metadata
                    Event* event = Event::GetByMetadata(methodMeta);
                    if(event != nullptr)
                    {
                        if(!Helpers::CheckEventFunctionParams(method)) continue;
                        // Store the event on the method
                        method->SetUserData(event);
                        continue;
                    }

                    std::smatch results;
                    // Check for remote custom events
                    auto result = std::regex_search(methodMeta.cbegin(), methodMeta.cend(), results, customEventRemoteRegex);
                    if(result)
                    {
                        if(!Helpers::CheckEventFunctionParams(method)) continue;
                        auto eventName = results[1].str();
                        // Store the custom event name on the method
                        method->SetUserData(new std::string(eventName), 2);
                        continue;
                    }
                    // Check for local custom events
                    result = std::regex_search(methodMeta.cbegin(), methodMeta.cend(), results, customEventLocalRegex);
                    if(result)
                    {
                        if(!Helpers::CheckEventFunctionParams(method)) continue;
                        auto eventName = results[1].str();
                        // Store the custom event name on the method
                        method->SetUserData(new std::string(eventName), 1);
                        continue;
                    }

                    if(methodMeta == "EveryTick")
                    {
                        CreateTimer(0, method, obj, false);
                        continue;
                    }
                    result = std::regex_search(methodMeta.cbegin(), methodMeta.cend(), results, intervalRegex);
                    if(result)
                    {
                        uint32_t interval;
                        try
                        {
                            interval = std::stoul(results[1].str());
                        }
                        catch(...)
                        {
                            Log::Error << "Failed to register interval on method '" << method->GetName() << "'" << Log::Endl;
                            continue;
                        }
                        CreateTimer(interval, method, obj, false);
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
            // todo: add import of functions from non angelscript resources
            /*
            auto funcInfo = Helpers::GetFunctionInfoFromDecl(decl);
            AS_ASSERT(funcInfo.valid, "Invalid function declaration specified", );

            for(auto it = exports->Begin(); it; it = exports->Next())
            {
                auto name = it->GetKey();
                if(name != functionName) continue;
                auto val = it->GetValue();
                if(val->GetType() != alt::IMValue::Type::FUNCTION) continue;

                auto               func = val.As<alt::MValueFunctionConst>();
                std::stringstream  code;
                asIScriptFunction* outFunc;

                code << decl << " {";
                code << "}";

                int r = module->CompileFunction(source, code.str().c_str(), 0, 0, &outFunc);
                CHECK_AS_RETURN("Import function compilation", r, );
                importedFunctions.push_back(outFunc);
                module->BindImportedFunction(i, outFunc);
            }
            */
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
