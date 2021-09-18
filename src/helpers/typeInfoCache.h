#pragma once
#include "angelscript/include/angelscript.h"
#include "Log.h"
#include <string>
#include <unordered_map>

namespace Helpers
{
    class TypeInfoCache
    {
        std::unordered_map<std::string, asITypeInfo*> cache;

    public:
        TypeInfoCache() {}
        TypeInfoCache(const TypeInfoCache&) = delete;
        TypeInfoCache(TypeInfoCache&&)      = delete;
        TypeInfoCache& operator=(const TypeInfoCache&) = delete;
        TypeInfoCache& operator=(TypeInfoCache&&) = delete;

        void Add(asIScriptEngine* engine, const std::string& name)
        {
            auto typeInfo = engine->GetTypeInfoByDecl(name.c_str());
            if(!typeInfo)
            {
                Log::Error << "TypeInfoCache: Type info with name '" << name << "' not found. Please report this!" << Log::Endl;
                return;
            }
            cache.insert({ name, typeInfo });
#ifdef DEBUG_MODE
            Log::Colored << "~lc~[DEBUG] Added cached type info for '~lk~" << name << "~lc~'" << Log::Endl;
#endif
        }

        asITypeInfo* Get(const std::string& name)
        {
            auto result = cache.find(name);
            if(result == cache.end()) return nullptr;
            return result->second;
        }
    };
}  // namespace Helpers
