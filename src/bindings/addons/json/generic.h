#pragma once
#include "nlohmann/json.hpp"
#include "helpers/angelscript.h"

namespace JSON
{   
    template<typename T>
    static void JsonConstructor(void* memory, T val)
    {
        new(memory) nlohmann::json(val);
    }
    static void JsonConstructorNull(void* memory)
    {
        new(memory) nlohmann::json();
    }
    static void JsonDestructor(void* memory)
    {
        ((nlohmann::json*)memory)->~basic_json();
    }

    static nlohmann::json Parse(const std::string& jsonString)
    {
        nlohmann::json parsed = nlohmann::json::parse(jsonString, nullptr, false, true);
        if(parsed.type() == nlohmann::json::value_t::discarded)
        {
            THROW_ERROR("Invalid JSON string");
            return nullptr;
        }
        return parsed;
    }
}
