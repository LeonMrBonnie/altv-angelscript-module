#include "discord.h"

#ifdef CLIENT_MODULE
static DataExtension discordDataExtension([](asIScriptEngine* engine, DocsGenerator* docs) {
    using namespace Data;
    REGISTER_VALUE_CLASS("DiscordData", DiscordData, asOBJ_VALUE | asOBJ_POD, "Discord data");
    REGISTER_PROPERTY("DiscordData", "const bool ready", DiscordData, ready);
    REGISTER_PROPERTY("DiscordData", "const string userID", DiscordData, userID);
    REGISTER_PROPERTY("DiscordData", "const string username", DiscordData, username);
    REGISTER_PROPERTY("DiscordData", "const string discriminator", DiscordData, discriminator);
    REGISTER_PROPERTY("DiscordData", "const string avatar", DiscordData, avatar);
});
#endif
