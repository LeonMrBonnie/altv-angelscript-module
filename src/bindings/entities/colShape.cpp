#include "Log.h"
#include "../../helpers/module.h"
#ifdef SERVER_MODULE
#include "colShape.h"

using namespace Helpers;

static alt::IColShape* SphereFactory(Vector3& pos, float radius)
{
    auto shape = alt::ICore::Instance().CreateColShapeSphere({pos.x, pos.y, pos.z}, radius);
    if(shape.IsEmpty())
    {
        THROW_ERROR("Failed to create colshape sphere");
        return nullptr;
    }
    return shape.Get();
}

static alt::IColShape* CylinderFactory(Vector3& pos, float radius, float height)
{
    auto shape = alt::ICore::Instance().CreateColShapeCylinder({pos.x, pos.y, pos.z}, radius, height);
    if(shape.IsEmpty())
    {
        THROW_ERROR("Failed to create colshape cylinder");
        return nullptr;
    }
    return shape.Get();
}

static alt::IColShape* CircleFactory(Vector3& pos, float radius)
{
    auto shape = alt::ICore::Instance().CreateColShapeCircle({pos.x, pos.y, pos.z}, radius);
    if(shape.IsEmpty())
    {
        THROW_ERROR("Failed to create colshape circle");
        return nullptr;
    }
    return shape.Get();
}

static alt::IColShape* CuboidFactory(Vector3& pos, Vector3& pos2)
{
    auto shape = alt::ICore::Instance().CreateColShapeCube({pos.x, pos.y, pos.z}, {pos2.x, pos2.y, pos2.z});
    if(shape.IsEmpty())
    {
        THROW_ERROR("Failed to create colshape cuboid");
        return nullptr;
    }
    return shape.Get();
}

static alt::IColShape* RectFactory(Vector2& pos, Vector2& pos2, float z)
{
    auto shape = alt::ICore::Instance().CreateColShapeRectangle(pos.x, pos.y, pos2.x, pos2.y, z);
    if(shape.IsEmpty())
    {
        THROW_ERROR("Failed to create colshape rect");
        return nullptr;
    }
    return shape.Get();
}

static alt::ICheckpoint* CheckpointFactory(uint8_t type, Vector3& pos, float radius, float height, alt::RGBA& color)
{
    auto shape = alt::ICore::Instance().CreateCheckpoint(type, {pos.x, pos.y, pos.z}, radius, height, color);
    if(shape.IsEmpty())
    {
        THROW_ERROR("Failed to create colshape checkpoint");
        return nullptr;
    }
    return shape.Get();
}

static ModuleExtension colshapeExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsColshape<alt::IColShape>(engine, docs, "ColShape");
    RegisterAsColshape<alt::IColShape>(engine, docs, "ColShapeSphere");
    RegisterAsColshape<alt::IColShape>(engine, docs, "ColShapeCylinder");
    RegisterAsColshape<alt::IColShape>(engine, docs, "ColShapeCircle");
    RegisterAsColshape<alt::IColShape>(engine, docs, "ColShapeCuboid");
    RegisterAsColshape<alt::IColShape>(engine, docs, "ColShapeRect");
    RegisterAsColshape<alt::ICheckpoint>(engine, docs, "Checkpoint");

    REGISTER_FACTORY("ColShapeSphere", "Vector3 pos, float radius", SphereFactory);
    REGISTER_FACTORY("ColShapeCylinder", "Vector3 pos, float radius, float height", CylinderFactory);
    REGISTER_FACTORY("ColShapeCircle", "Vector3 pos, float radius", CircleFactory);
    REGISTER_FACTORY("ColShapeCuboid", "Vector3 pos, Vector3 pos2", CuboidFactory);
    REGISTER_FACTORY("ColShapeRect", "Vector2 pos, Vector2 pos2, float z", RectFactory);
    REGISTER_FACTORY("Checkpoint", "uint8 type, Vector3 pos, float radius, float height, RGBA color", CheckpointFactory);

    // Checkpoint
    // todo: add mising checkpoint methods and properties
});
#endif