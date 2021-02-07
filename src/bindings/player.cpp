#pragma once
#include "Log.h"
#include "../helpers/module.h"
#include "entity.h"

using namespace Helpers;

static std::string GetName(alt::IPlayer* player)
{
    return player->GetName().CStr();
}

template<class T>
static void SpawnPlayer(T x, T y, T z, uint32_t delay, alt::IPlayer* player)
{
    player->Spawn({x, y, z}, delay);
}

static void SetModel(uint32_t model, alt::IPlayer* player)
{
    player->SetModel(model);
}

static ModuleExtension playerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_REF_CLASS("Player", alt::IPlayer, asOBJ_REF, "alt:V Player Entity");
    RegisterAsEntity<alt::IPlayer>(engine, docs, "Player");

    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "name", GetName);
    REGISTER_METHOD_WRAPPER("Player", "string GetName()", GetName);

    REGISTER_METHOD_WRAPPER("Player", "void Spawn(float x, float y, float z, uint delay)", SpawnPlayer<float>);
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(int x, int y, int z, uint delay)", SpawnPlayer<int>);

    REGISTER_METHOD_WRAPPER("Player", "void SetModel(uint model)", SetModel);

    // todo: add missing methods
});
