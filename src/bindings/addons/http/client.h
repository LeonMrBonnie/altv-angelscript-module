#pragma once
#include "runtime.h"
#include "angelscript/add_on/scriptdictionary/scriptdictionary.h"

#include "httplib/httplib.h"

namespace HTTP
{
    static void Construct(void* memory, std::string& url)
    {
        new(memory) httplib::Client(url.c_str());
    }

    static void Destruct(void* memory)
    {
        ((httplib::Client*)memory)->~Client();
    }

    static httplib::Result GetRequest(httplib::Client* client, const std::string& path, CScriptDictionary* headersDict)
    {
        const int        stringTypeId = AngelScriptRuntime::Instance().GetTypeInfoCache().Get("string")->GetTypeId();
        httplib::Headers headers;
        for(auto it : *headersDict)
        {
            if(it.GetTypeId() != stringTypeId) continue;
            headers.insert({ it.GetKey(), *(std::string*)it.GetAddressOfValue() });
        }

        return client->Get(path.c_str(), headers);
    }

    static httplib::Result PostRequest(httplib::Client* client, const std::string& path, CScriptDictionary* paramsDict)
    {
        const int       stringTypeId = AngelScriptRuntime::Instance().GetTypeInfoCache().Get("string")->GetTypeId();
        httplib::Params params;
        for(auto it : *paramsDict)
        {
            if(it.GetTypeId() != stringTypeId) continue;
            params.insert({ it.GetKey(), *(std::string*)it.GetAddressOfValue() });
        }

        return client->Post(path.c_str(), params);
    }

    static httplib::Result PutRequest(httplib::Client* client, const std::string& path, CScriptDictionary* paramsDict)
    {
        const int       stringTypeId = AngelScriptRuntime::Instance().GetTypeInfoCache().Get("string")->GetTypeId();
        httplib::Params params;
        for(auto it : *paramsDict)
        {
            if(it.GetTypeId() != stringTypeId) continue;
            params.insert({ it.GetKey(), *(std::string*)it.GetAddressOfValue() });
        }

        return client->Put(path.c_str(), params);
    }

    static httplib::Result DeleteRequest(httplib::Client* client, const std::string& path, CScriptDictionary* headersDict)
    {
        const int        stringTypeId = AngelScriptRuntime::Instance().GetTypeInfoCache().Get("string")->GetTypeId();
        httplib::Headers headers;
        for(auto it : *headersDict)
        {
            if(it.GetTypeId() != stringTypeId) continue;
            headers.insert({ it.GetKey(), *(std::string*)it.GetAddressOfValue() });
        }

        return client->Delete(path.c_str(), headers);
    }
}  // namespace HTTP
