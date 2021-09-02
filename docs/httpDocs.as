// Generated on Fri Jul 16 02:40:48 2021

namespace http
{
    // ********** Enums **********

    // A HTTP request error
    enum Error
    {
        SUCCESS = 0,
        UNKNOWN = 1,
        CONNECTION = 2,
        BIND_IP_ADDRESS = 3,
        READ = 4,
        WRITE = 5,
        EXCEED_REDIRECT_COUNT = 6,
        CANCELED = 7,
        SSL_CONNECTION = 8,
        SSL_LOADING_CERTS = 9,
        SSL_SERVER_VERIFICATION = 10,
        UNSUPPORTED_MULTIPART_BOUNDARY_CHARS = 11,
        COMPRESSION = 12,
    };

    // ********** Objects **********

    // A result from a HTTP request
    class Result
    {
        http::Error error;
        int status;
        string body;

    };

    // An instance of the HTTP client to make HTTP requests
    class Client
    {

        Client(const string&in url);
        Result Get(const string&in path, dictionary@ headers = {});
        Result Post(const string&in path, dictionary@ params = {});
        Result Put(const string&in path, dictionary@ params = {});
        Result Delete(const string&in path, dictionary@ headers = {});
    };
}
