#pragma once
#include "Log.h"
#include "../helpers/module.h"
#include "entity.h"

using namespace Helpers;

static std::string GetName(alt::IPlayer* player)
{
    return player->GetName().ToString();
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

static std::string ToString(alt::IPlayer* player)
{
    std::stringstream str;
    str << "Player{ id: " << std::to_string(player->GetID()) << ", name: " << player->GetName().ToString() << " }";
    return str.str();
}

static ModuleExtension playerExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsEntity<alt::IPlayer>(engine, docs, "Player");

    // Implicit conversion to string
    REGISTER_METHOD_WRAPPER("Player", "string opImplConv() const", ToString);

    REGISTER_PROPERTY_WRAPPER_GET("Player", "string", "name", GetName);
    REGISTER_PROPERTY_WRAPPER_SET("Player", "uint", "model", SetModel);
    
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(float x, float y, float z, uint delay)", SpawnPlayer<float>);
    REGISTER_METHOD_WRAPPER("Player", "void Spawn(int x, int y, int z, uint delay)", SpawnPlayer<int>);

    // todo: add missing methods
});
