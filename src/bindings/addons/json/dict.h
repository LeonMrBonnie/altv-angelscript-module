#pragma once
#include "nlohmann/json.hpp"
#include "helpers/angelscript.h"
#include "angelscript/add_on/scriptdictionary/scriptdictionary.h"

namespace JSON
{
    static void JsonConstructorDict(void* memory, CScriptDictionary* val)
    {
        using namespace nlohmann;
        static int jsonTypeId = AngelScriptRuntime::Instance().GetEngine()->GetTypeIdByDecl("json::JSON");

        std::unordered_map<std::string, json> map;
        for(auto it : *val)
        {
            switch(it.GetTypeId())
            {
                case asTYPEID_BOOL:
                {
                    map.insert({ it.GetKey(), *(bool*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_INT8:
                {
                    map.insert({ it.GetKey(), *(int8_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_INT16:
                {
                    map.insert({ it.GetKey(), *(int16_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_INT32:
                {
                    map.insert({ it.GetKey(), *(int32_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_INT64:
                {
                    map.insert({ it.GetKey(), *(int64_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_UINT8:
                {
                    map.insert({ it.GetKey(), *(uint8_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_UINT16:
                {
                    map.insert({ it.GetKey(), *(uint16_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_UINT32:
                {
                    map.insert({ it.GetKey(), *(uint32_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_UINT64:
                {
                    map.insert({ it.GetKey(), *(uint64_t*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_FLOAT:
                {
                    map.insert({ it.GetKey(), *(float*)it.GetAddressOfValue() });
                    break;
                }
                case asTYPEID_DOUBLE:
                {
                    map.insert({ it.GetKey(), *(double*)it.GetAddressOfValue() });
                    break;
                }
                default:
                {
                    if(it.GetTypeId() == jsonTypeId) map.insert({ it.GetKey(), *(json*)it.GetAddressOfValue() });
                    if(it.GetTypeId() == AngelScriptRuntime::Instance().GetTypeInfoCache().Get("string")->GetTypeId())
                        map.insert({ it.GetKey(), *(std::string*)it.GetAddressOfValue() });
                    break;
                }
            }
        }
        new(memory) json(map);
    }
}  // namespace JSON
