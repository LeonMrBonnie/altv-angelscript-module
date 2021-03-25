#include "helpers/module.h"
#include "runtime.h"
#include "angelscript/addon/scriptdictionary/scriptdictionary.h"

#include "httplib/httplib.h"

using namespace Helpers;

// *** Result class
static httplib::Error GetResultError(httplib::Result* result)
{
    return result->error();
}

static int GetResultStatusCode(httplib::Result* result)
{
    return (*result)->status;
}

static std::string GetResultBody(httplib::Result* result)
{
    return (*result)->body;
}

/// *** Client class
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
    const int stringTypeId = AngelScriptRuntime::Instance().GetStringTypeId();
    httplib::Headers headers;
    for(auto it : *headersDict)
    {
        if(it.GetTypeId() != stringTypeId) continue;
        headers.insert({it.GetKey(), *(std::string*)it.GetAddressOfValue()});
    }

    return client->Get(path.c_str(), headers);
}

static httplib::Result PostRequest(httplib::Client* client, const std::string& path, CScriptDictionary* paramsDict)
{
    const int stringTypeId = AngelScriptRuntime::Instance().GetStringTypeId();
    httplib::Params params;
    for(auto it : *paramsDict)
    {
        if(it.GetTypeId() != stringTypeId) continue;
        params.insert({it.GetKey(), *(std::string*)it.GetAddressOfValue()});
    }

    return client->Post(path.c_str(), params);
}

static httplib::Result PutRequest(httplib::Client* client, const std::string& path, CScriptDictionary* paramsDict)
{
    const int stringTypeId = AngelScriptRuntime::Instance().GetStringTypeId();
    httplib::Params params;
    for(auto it : *paramsDict)
    {
        if(it.GetTypeId() != stringTypeId) continue;
        params.insert({it.GetKey(), *(std::string*)it.GetAddressOfValue()});
    }

    return client->Put(path.c_str(), params);
}

static httplib::Result DeleteRequest(httplib::Client* client, const std::string& path, CScriptDictionary* headersDict)
{
    const int stringTypeId = AngelScriptRuntime::Instance().GetStringTypeId();
    httplib::Headers headers;
    for(auto it : *headersDict)
    {
        if(it.GetTypeId() != stringTypeId) continue;
        headers.insert({it.GetKey(), *(std::string*)it.GetAddressOfValue()});
    }

    return client->Delete(path.c_str(), headers);
}

static ModuleExtension httpExtension("http", [](asIScriptEngine* engine, DocsGenerator* docs) {
    // Error enum
    REGISTER_ENUM("Error", "A HTTP request error");
    REGISTER_ENUM_VALUE("Error", "SUCCESS", httplib::Error::Success);

    // Result class
    REGISTER_VALUE_CLASS("Result", httplib::Result, asOBJ_VALUE | asOBJ_POD, "A result from a HTTP request");
    REGISTER_PROPERTY_WRAPPER_GET("Result", "http::Error", "error", GetResultError);
    REGISTER_PROPERTY_WRAPPER_GET("Result", "int", "status", GetResultStatusCode);
    REGISTER_PROPERTY_WRAPPER_GET("Result", "string", "body", GetResultBody);

    // Client class
    REGISTER_VALUE_CLASS("Client", httplib::Client, asOBJ_VALUE, "An instance of the HTTP client to make HTTP requests");
    REGISTER_CONSTRUCTOR("Client", "const string&in url", Construct);
    REGISTER_DESTRUCTOR("Client", Destruct);
    
    REGISTER_METHOD_WRAPPER("Client", "Result Get(const string&in path, dictionary@ headers = {})", GetRequest);
    REGISTER_METHOD_WRAPPER("Client", "Result Post(const string&in path, dictionary@ params = {})", PostRequest);
    REGISTER_METHOD_WRAPPER("Client", "Result Put(const string&in path, dictionary@ params = {})", PutRequest);
    REGISTER_METHOD_WRAPPER("Client", "Result Delete(const string&in path, dictionary@ headers = {})", DeleteRequest);
});
