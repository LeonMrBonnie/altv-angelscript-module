#include "helpers/module.h"
#include "./result.h"
#include "./client.h"

using namespace Helpers;

static ModuleExtension httpExtension("http", [](asIScriptEngine* engine, DocsGenerator* docs) {
    using namespace HTTP;

    // Error enum
    REGISTER_ENUM("Error", "A HTTP request error");
    REGISTER_ENUM_VALUE("Error", "SUCCESS", httplib::Error::Success);
    REGISTER_ENUM_VALUE("Error", "UNKNOWN", httplib::Error::Unknown);
    REGISTER_ENUM_VALUE("Error", "CONNECTION", httplib::Error::Connection);
    REGISTER_ENUM_VALUE("Error", "BIND_IP_ADDRESS", httplib::Error::BindIPAddress);
    REGISTER_ENUM_VALUE("Error", "READ", httplib::Error::Read);
    REGISTER_ENUM_VALUE("Error", "WRITE", httplib::Error::Write);
    REGISTER_ENUM_VALUE("Error", "EXCEED_REDIRECT_COUNT", httplib::Error::ExceedRedirectCount);
    REGISTER_ENUM_VALUE("Error", "CANCELED", httplib::Error::Canceled);
    REGISTER_ENUM_VALUE("Error", "SSL_CONNECTION", httplib::Error::SSLConnection);
    REGISTER_ENUM_VALUE("Error", "SSL_LOADING_CERTS", httplib::Error::SSLLoadingCerts);
    REGISTER_ENUM_VALUE("Error", "SSL_SERVER_VERIFICATION", httplib::Error::SSLServerVerification);
    REGISTER_ENUM_VALUE("Error", "UNSUPPORTED_MULTIPART_BOUNDARY_CHARS", httplib::Error::UnsupportedMultipartBoundaryChars);
    REGISTER_ENUM_VALUE("Error", "COMPRESSION", httplib::Error::Compression);

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
