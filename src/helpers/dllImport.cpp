#include "dllImport.h"
#include "resource.h"
#include "./convert.h"
#include "./angelscript.h"

#ifdef SERVER_MODULE

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        #include "windows.h"
        #define _getlib(name)                LoadLibraryA(name)
        #define _getfunc(module, name, type) (type) GetProcAddress(module, name);
        #define _seperator                   "\\"
    /*
    #else
        #include <dlfcn.h>
        #define _getlib(name)                dlopen(name, RTLD_NOW);
        #define _getfunc(module, name, type) (type) dlsym(module, name);
        #define _seperator                   "/"
    */
    #endif

    #include <algorithm>
    #include <cctype>
    #include <locale>

static inline void CleanString(std::string& str)
{
    str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
    str.erase(std::remove(str.begin(), str.end(), '\''), str.end());
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), str.end());
}

static void DoDllImport(std::string& pragma, AngelScriptResource* resource)
{
    auto parts = Helpers::SplitString(pragma, ",", 3);
    for(auto& part : parts)
    {
        CleanString(part);
    }

    std::string dllName       = parts[0];
    std::string funcNamespace = parts[1];
    std::string funcDecl      = parts[2];

    std::string dllPath = resource->GetIResource()->GetPath().ToString();
    dllPath += _seperator;
    dllPath += dllName;

    auto dll = _getlib(dllPath.c_str());
    if(dll == nullptr)
    {
        Log::Error << "Failed to load dll '" << dllName << "'" << Log::Endl;
        return;
    }

    auto funcInfo = Helpers::GetFunctionInfoFromDecl(funcDecl);
    auto dllFunc  = _getfunc(dll, funcInfo.functionName.c_str(), void*);
    if(dllFunc == nullptr)
    {
        Log::Error << "Failed to find dll function '" << funcInfo.functionName << "' in dll '" << dllName << "'" << Log::Endl;
        return;
    }

    resource->GetRuntime()->GetEngine()->SetDefaultNamespace(funcNamespace.c_str());
    int createdFuncId = resource->GetRuntime()->GetEngine()->RegisterGlobalFunction(funcDecl.c_str(), asFUNCTION(dllFunc), asCALL_CDECL);
    if(createdFuncId < 0)
    {
        Log::Error << "Failed to create global dll function '" << funcInfo.functionName << "' Error: " << createdFuncId << Log::Endl;
        return;
    }
    asIScriptFunction* createdFunc = resource->GetRuntime()->GetEngine()->GetFunctionById(createdFuncId);
    resource->AddDllImportFunction(createdFunc);
}

bool DllImport::DllImportPragmaHandler(const std::string& pragmaStr, AngelScriptResource* resource)
{
    std::smatch results;

    if(std::regex_search(pragmaStr.cbegin(), pragmaStr.cend(), results, std::regex("dllImport\\((.*)\\)")))
    {
        DoDllImport(results[1].str(), resource);
        return true;
    }

    return false;
}
#else
bool DllImport::DllImportPragmaHandler(const std::string& pragmaStr, AngelScriptResource* resource)
{
    return false;
}
#endif
