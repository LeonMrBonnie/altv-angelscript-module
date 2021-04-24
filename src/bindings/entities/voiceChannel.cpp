#include "Log.h"
#include "../../helpers/module.h"
#include "baseObject.h"

#ifdef SERVER_MODULE
using namespace Helpers;

static std::string ToString(alt::IVoiceChannel* channel)
{
    std::stringstream str;
    str << "VoiceChannel{ }";
    return str.str();
}

static alt::IVoiceChannel* Factory(bool spatial = false, float maxDistance = 0)
{
    auto channel = alt::ICore::Instance().CreateVoiceChannel(spatial, maxDistance);
    if(channel.IsEmpty())
    {
        THROW_ERROR("Failed to create the voice channel, the voice chat is not enabled");
        return nullptr;
    }
    return channel.Get();
}

static ModuleExtension voiceChannelExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsBaseObject<alt::IVoiceChannel>(engine, docs, "VoiceChannel");

    REGISTER_FACTORY("VoiceChannel", "bool isSpatial = false, float maxDistance = 0", Factory);

    // Implicit conversion to string
    REGISTER_METHOD_WRAPPER("VoiceChannel", "string opImplConv() const", ToString);

    REGISTER_PROPERTY_WRAPPER_GET(
      "VoiceChannel", "bool", "spatial", (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::IsSpatial, bool>));
    REGISTER_PROPERTY_WRAPPER_GET(
      "VoiceChannel", "float", "maxDistance", (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::GetMaxDistance, float>));

    REGISTER_METHOD_WRAPPER("VoiceChannel",
                            "bool HasPlayer(Player@ player)",
                            (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::HasPlayer, bool, alt::IPlayer*>));
    REGISTER_METHOD_WRAPPER("VoiceChannel",
                            "void AddPlayer(Player@ player)",
                            (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::AddPlayer, void, alt::IPlayer*>));
    REGISTER_METHOD_WRAPPER("VoiceChannel",
                            "void RemovePlayer(Player@ player)",
                            (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::RemovePlayer, void, alt::IPlayer*>));

    REGISTER_METHOD_WRAPPER("VoiceChannel",
                            "bool IsPlayerMuted(Player@ player)",
                            (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::IsPlayerMuted, bool, alt::IPlayer*>));
    REGISTER_METHOD_WRAPPER("VoiceChannel",
                            "void MutePlayer(Player@ player)",
                            (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::MutePlayer, void, alt::IPlayer*>));
    REGISTER_METHOD_WRAPPER("VoiceChannel",
                            "void UnmutePlayer(Player@ player)",
                            (GenericWrapper<alt::IVoiceChannel, alt::IVoiceChannel, &alt::IVoiceChannel::UnmutePlayer, void, alt::IPlayer*>));
});
#endif