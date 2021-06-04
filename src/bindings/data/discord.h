#pragma once
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

namespace Data
{
    struct DiscordData
    {
        bool        ready;
        std::string userID;
        std::string username;
        std::string discriminator;
        std::string avatar;
    };

}  // namespace Data
