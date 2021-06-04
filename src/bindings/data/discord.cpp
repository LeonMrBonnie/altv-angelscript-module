#include "discord.h"

#ifdef CLIENT_MODULE
static DataExtension discordDataExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    using namespace Data;
    REGISTER_VALUE_CLASS("DiscordData", DiscordData, asOBJ_VALUE | asOBJ_POD, "Discord data");
    REGISTER_PROPERTY("DiscordData", "bool ready", DiscordData, ready);
    REGISTER_PROPERTY("DiscordData", "string userID", DiscordData, userID);
    REGISTER_PROPERTY("DiscordData", "string username", DiscordData, username);
    REGISTER_PROPERTY("DiscordData", "string discriminator", DiscordData, discriminator);
    REGISTER_PROPERTY("DiscordData", "string avatar", DiscordData, avatar);
});
#endif
