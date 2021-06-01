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

    static void RegisterDiscordData(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("DiscordData", DiscordData, asOBJ_VALUE | asOBJ_POD, "Discord data");
        REGISTER_PROPERTY("DiscordData", "bool ready", DiscordData, ready);
        REGISTER_PROPERTY("DiscordData", "string userID", DiscordData, userID);
        REGISTER_PROPERTY("DiscordData", "string username", DiscordData, username);
        REGISTER_PROPERTY("DiscordData", "string discriminator", DiscordData, discriminator);
        REGISTER_PROPERTY("DiscordData", "string avatar", DiscordData, avatar);
    }
}  // namespace Data
