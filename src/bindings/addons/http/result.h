#pragma once
#include "httplib/httplib.h"

namespace HTTP
{
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
}
