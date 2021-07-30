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
    /*
    #else
        #include <dlfcn.h>
        #define _getlib(name)                dlopen((name + ".so").c_str(), RTLD_NOW);
        #define _getfunc(module, name, type) (type) dlsym(module, name);
        #define _unloadlib(lib)              dlclose(lib)
        #define _seperator                   "/"
    */
    #endif

    #include <algorithm>
    #include <cctype>
    #include <locale>

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

static inline void* ImportLibrary(std::string& name, AngelScriptResource* resource)
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

static void DoDllImport(std::string& pragma, AngelScriptResource* resource)
{
    GET_PRAGMA_PARTS(pragma, 3, "Error in libImport pragma: Needs 3 arguments (library, namespace, function declaration)");

    std::string libName       = parts[0];
    std::string funcNamespace = parts[1];
    std::string funcDecl      = parts[2];

    auto lib = ImportLibrary(libName, resource);

    auto funcInfo = Helpers::GetFunctionInfoFromDecl(funcDecl);
    auto dllFunc  = _getfunc(lib, funcInfo.functionName.c_str(), void*);
    if(dllFunc == nullptr)
    {
        Log::Error << "Failed to find export '" << funcInfo.functionName << "' in lib '" << libName << "'" << Log::Endl;
        return;
    }

    resource->GetRuntime()->GetEngine()->SetDefaultNamespace(funcNamespace.c_str());
    int createdFuncId = resource->GetRuntime()->GetEngine()->RegisterGlobalFunction(funcDecl.c_str(), asFUNCTION(dllFunc), asCALL_CDECL);
    if(createdFuncId < 0)
    {
        Log::Error << "Failed to create global lib function '" << funcInfo.functionName << "' Error: " << createdFuncId << Log::Endl;
        return;
    }
    asIScriptFunction* createdFunc = resource->GetRuntime()->GetEngine()->GetFunctionById(createdFuncId);
    resource->AddLibraryImportFunction(createdFunc, lib);
}

static void DoBeginStruct(std::string& pragma, AngelScriptResource* resource)
{
    GET_PRAGMA_PARTS(pragma, 4, "Error in beginStruct pragma: Needs 4 arguments (dll, namespace, name, size)");

    std::string libName         = parts[0];
    std::string structNamespace = parts[1];
    std::string structName      = parts[2];
    int         size            = std::stoi(parts[3]);

    if(size < 1)
    {
        Log::Error << "Error in beginStruct pragma: Invalid size specified (must be more than 0)" << Log::Endl;
        return;
    }

    auto lib = ImportLibrary(libName, resource);
    // todo: finish this
}

// ********** Functionality **********

bool LibraryImport::LibraryImportPragmaHandler(const std::string& pragmaStr, AngelScriptResource* resource)
{
    std::smatch results;

    if(std::regex_search(pragmaStr.cbegin(), pragmaStr.cend(), results, std::regex("libImport\\((.*)\\)")))
    {
        DoDllImport(results[1].str(), resource);
        return true;
    }
    if(std::regex_search(pragmaStr.cbegin(), pragmaStr.cend(), results, std::regex("beginStruct\\((.*)\\)")))
    {
        DoBeginStruct(results[1].str(), resource);
        return true;
    }

    return false;
}

void LibraryImport::FreeLibrary(void* dll)
{
    _unloadlib(dll);
}
#endif
#ifdef CLIENT_MODULE
bool LibraryImport::LibraryImportPragmaHandler(const std::string& pragmaStr, AngelScriptResource* resource)
{
    return false;
}
void LibraryImport::FreeLibrary(void* dll) {}
#endif
