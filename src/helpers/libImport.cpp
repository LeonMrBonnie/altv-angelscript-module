#include "libImport.h"
#include "resource.h"
#include "./convert.h"
#include "./angelscript.h"

#ifdef SERVER_MODULE

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        #include "windows.h"
        #define _getlib(name)                LoadLibraryA((name + ".dll").c_str())
        #define _getfunc(module, name, type) (type) GetProcAddress((HMODULE)module, name);
        #define _unloadlib(lib)              FreeLibrary((HMODULE)lib)
        #define _seperator                   "\\"
    #else
        #include <dlfcn.h>
        #define _getlib(name)                dlopen((name + ".so").c_str(), 0x00002);
        #define _getfunc(module, name, type) (type) dlsym(module, name);
        #define _unloadlib(lib)              dlclose(lib)
        #define _seperator                   "/"
    #endif

    #include <algorithm>
    #include <cctype>
    #include <locale>
    #include <map>

// ********** Helpers **********

    #define GET_PRAGMA_PARTS(pragmaVal, partsSize, error)             \
        auto parts = Helpers::SplitString(pragmaVal, ",", partsSize); \
        if(parts.size() != partsSize)                                 \
        {                                                             \
            Log::Error << error << Log::Endl;                         \
            return;                                                   \
        }                                                             \
        for(auto& part : parts)                                       \
        {                                                             \
            CleanString(part);                                        \
        }

static inline void CleanString(std::string& str)
{
    str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\''), str.end());
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), str.end());
}

static inline void* ImportLibrary(std::string& name, const AngelScriptResource* resource)
{
    std::string libPath = resource->GetIResource()->GetPath().ToString();
    libPath += _seperator;
    libPath += name;
    auto dll = _getlib(libPath);
    if(dll == nullptr)
    {
        Log::Error << "Failed to load library '" << name << "'" << Log::Endl;
    }
    return dll;
}

// ********** Pragma Handlers **********

static void DoLibImport(std::string pragma, const AngelScriptResource* resource)
{
    GET_PRAGMA_PARTS(pragma, 3, "Error in libImport pragma: Needs 3 arguments (library, namespace, function declaration)");

    std::string libName       = parts[0];
    std::string funcNamespace = parts[1];
    std::string funcDecl      = parts[2];
    if(libName.size() == 0)
    {
        Log::Error << "Error in libImport pragma: Invalid library name" << Log::Endl;
        return;
    }

    auto lib = ImportLibrary(libName, resource);
    if(!lib)
    {
        Log::Error << "Error in libImport pragma: Library not found" << Log::Endl;
        return;
    }

    auto funcInfo = Helpers::GetFunctionInfoFromDecl(funcDecl);
    if(!funcInfo.valid)
    {
        Log::Error << "Error in libImport pragma: Invalid function declaration" << Log::Endl;
        return;
    }
    auto dllFunc = _getfunc(lib, funcInfo.functionName.c_str(), void*);
    if(dllFunc == nullptr)
    {
        Log::Error << "Error in libImport pragma: Failed to find export '" << funcInfo.functionName << "' in lib '" << libName << "'" << Log::Endl;
        return;
    }

    resource->GetRuntime()->GetEngine()->SetDefaultNamespace(funcNamespace.c_str());
    int createdFuncId = resource->GetRuntime()->GetEngine()->RegisterGlobalFunction(funcDecl.c_str(), asFUNCTION(dllFunc), asCALL_CDECL);
    if(createdFuncId < 0)
    {
        Log::Error << "Error in libImport pragma: Failed to create global lib function '" << funcInfo.functionName << "' Error: " << createdFuncId
                   << Log::Endl;
        return;
    }
    asIScriptFunction* createdFunc = resource->GetRuntime()->GetEngine()->GetFunctionById(createdFuncId);
    const_cast<AngelScriptResource*>(resource)->AddLibraryImportFunction(createdFunc, lib);

    #ifdef DEBUG_MODE
    Log::Warning << __FUNCTION__ << " "
                 << "Imported lib function '" << funcInfo.functionName << "' from lib '" << libName << "'" << Log::Endl;
    #endif
}

struct ScriptStruct
{
    ScriptStruct(uint32_t size) : size(size) {}

    uint32_t size;
    // Current byte offset
    uint32_t curOffset = 0;
};
// Key = Name
static std::unordered_map<std::string, ScriptStruct> scriptStructs;

static void DoDeclareStruct(std::string pragma)
{
    GET_PRAGMA_PARTS(pragma, 4, "Error in declareStruct pragma: Needs 4 arguments (library, namespace, name, size)");

    std::string libName         = parts[0];
    std::string structNamespace = parts[1];
    std::string structName      = parts[2];
    int         size            = std::stoi(parts[3]);
    if(size < 1)
    {
        Log::Error << "Error in declareStruct pragma: Invalid size specified (must be more than 0)" << Log::Endl;
        return;
    }
    if(scriptStructs.count(structName) != 0)
    {
        Log::Error << "Error in declareStruct pragma: Struct already exists" << Log::Endl;
        return;
    }

    AngelScriptRuntime::Instance().GetEngine()->SetDefaultNamespace(structNamespace.c_str());
    int result = AngelScriptRuntime::Instance().GetEngine()->RegisterObjectType(structName.c_str(), size, asOBJ_VALUE);
    switch(result)
    {
        case asINVALID_NAME:
        {
            Log::Error << "Error in declareStruct pragma: Invalid name" << Log::Endl;
            return;
        }
        case asALREADY_REGISTERED:
        {
            Log::Error << "Error in declareStruct pragma: Struct with that name already registered" << Log::Endl;
            return;
        }
        case asNAME_TAKEN:
        {
            Log::Error << "Error in declareStruct pragma: Name already taken" << Log::Endl;
            return;
        }
    }

    scriptStructs.insert({ structName, ScriptStruct(size) });

    #ifdef DEBUG_MODE
    Log::Warning << __FUNCTION__ << " "
                 << "Created struct '" << structName << "' with size " << size << " from lib '" << libName << "'" << Log::Endl;
    #endif
}

static void DoStructProperty(std::string pragma)
{
    GET_PRAGMA_PARTS(pragma, 3, "Error in structProperty pragma: Needs 3 arguments (struct name, property name, type)");

    std::string structName   = parts[0];
    std::string propertyName = parts[1];
    std::string type         = parts[2];
    if(scriptStructs.count(structName) == 0)
    {
        Log::Error << "Error in structProperty pragma: Struct does not exist (" << structName << ")" << Log::Endl;
        return;
    }
    if(propertyName.size() == 0)
    {
        Log::Error << "Error in structProperty pragma: Invalid property name (" << propertyName << ")" << Log::Endl;
        return;
    }
    if(type == structName)
    {
        Log::Error << "Error in structProperty pragma: Recursive property (" << propertyName << ")" << Log::Endl;
        return;
    }
    int typeId = Helpers::GetTypeIdFromName(type);
    if(typeId == -1)
    {
        Log::Error << "Error in structProperty pragma: Invalid type name (" << type << ")" << Log::Endl;
        return;
    }
    auto size         = Helpers::GetTypeSize(typeId);
    auto scriptStruct = scriptStructs.at(structName);
    if(scriptStruct.curOffset + size > scriptStruct.size)
    {
        Log::Error << "Error in structProperty pragma: Struct size is too small for type (Size: " << scriptStruct.size << ", Type Size: " << size
                   << ")" << Log::Endl;
        return;
    }

    auto decl   = type + " " + propertyName;
    int  result = AngelScriptRuntime::Instance().GetEngine()->RegisterObjectProperty(structName.c_str(), decl.c_str(), scriptStruct.curOffset);
    switch(result)
    {
        case asWRONG_CONFIG_GROUP:
        {
            Log::Error << "Error in structProperty pragma: Struct is not available in this resource" << Log::Endl;
            return;
        }
        case asINVALID_OBJECT:
        {
            Log::Error << "Error in structProperty pragma: Struct is not a struct" << Log::Endl;
            return;
        }
        case asINVALID_TYPE:
        {
            Log::Error << "Error in structProperty pragma: Struct is invalid" << Log::Endl;
            return;
        }
        case asINVALID_DECLARATION:
        {
            Log::Error << "Error in structProperty pragma: Invalid type or property name" << Log::Endl;
            return;
        }
        case asNAME_TAKEN:
        {
            Log::Error << "Error in structProperty pragma: Property name is already taken" << Log::Endl;
            return;
        }
    }

    #ifdef DEBUG_MODE
    Log::Warning << __FUNCTION__ << " "
                 << "Created struct property '" << propertyName << "' with size " << Helpers::GetTypeSize(typeId) << " at offset "
                 << scriptStruct.curOffset << " to struct '" << structName << "'" << Log::Endl;
    #endif

    scriptStruct.curOffset += Helpers::GetTypeSize(typeId);
}

static void DoEndStruct(std::string pragma)
{
    GET_PRAGMA_PARTS(pragma, 1, "Error in endStruct pragma: Needs 1 argument (struct name)");

    std::string structName = parts[0];
    if(scriptStructs.count(structName) == 0)
    {
        Log::Error << "Error in endStruct pragma: Struct does not exist (" << structName << ")" << Log::Endl;
        return;
    }

    scriptStructs.erase(structName);

    #ifdef DEBUG_MODE
    Log::Warning << __FUNCTION__ << " "
                 << "Ended struct '" << structName << "'" << Log::Endl;
    #endif
}

// ********** Functionality **********

    #define PRAGMA_CASE(name) std::regex_search(pragmaStr.cbegin(), pragmaStr.cend(), results, std::regex(name "\\((.*)\\)"))
bool LibraryImport::LibraryImportPragmaHandler(const std::string& pragmaStr, const AngelScriptResource* resource)
{
    std::smatch results;

    if(PRAGMA_CASE("libImport"))
    {
        DoLibImport(results[1].str(), resource);
        return true;
    }
    if(PRAGMA_CASE("declareStruct"))
    {
        DoDeclareStruct(results[1].str());
        return true;
    }
    if(PRAGMA_CASE("structProperty"))
    {
        DoStructProperty(results[1].str());
        return true;
    }
    if(PRAGMA_CASE("endStruct"))
    {
        DoEndStruct(results[1].str());
        return true;
    }

    return false;
}

void LibraryImport::FreeLibrary(void* library)
{
    _unloadlib(library);
}
#endif
#ifdef CLIENT_MODULE
bool LibraryImport::LibraryImportPragmaHandler(const std::string& pragmaStr, const AngelScriptResource* resource)
{
    return false;
}
void LibraryImport::FreeLibrary(void*) {}
#endif
