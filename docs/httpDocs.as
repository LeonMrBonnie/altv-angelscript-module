// Generated on Thu Mar 25 15:14:10 2021

namespace http
{
    // ********** Enums **********

    // A HTTP request error
    enum Error
    {
        SUCCESS = 0,
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
