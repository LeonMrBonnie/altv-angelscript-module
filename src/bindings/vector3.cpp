#include "Log.h"
#include "../helpers/module.h"

using namespace Helpers;

template<typename T>
class Vector3
{
public:
    T x;
    T y;
    T z;

    Vector3(T x, T y, T z) : x(x), y(y), z(z) {};

    float Length()
    {
        return sqrt(x * x + y * y + z * z);
    }

    static void Construct(T x, T y, T z, void* memory)
    {
        new(memory) Vector3<T>(x, y, z);
    }
};

static ModuleExtension vector3Extension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    REGISTER_CLASS("Vector3f", Vector3<float>, asOBJ_VALUE | asOBJ_POD, "Three-dimensional float vector");
    REGISTER_CONSTRUCTOR("Vector3f", "float x, float y, float z", Vector3<float>::Construct);
    REGISTER_PROPERTY("Vector3f", "float x", Vector3<float>, x);
    REGISTER_PROPERTY("Vector3f", "float y", Vector3<float>, y);
    REGISTER_PROPERTY("Vector3f", "float z", Vector3<float>, z);
    REGISTER_METHOD("Vector3f", "float Length()", Vector3<float>, Length);

    REGISTER_CLASS("Vector3i", Vector3<int>, asOBJ_VALUE | asOBJ_POD, "Three-dimensional integer vector");
    REGISTER_CONSTRUCTOR("Vector3i", "int x, int y, int z", Vector3<int>::Construct);
    REGISTER_PROPERTY("Vector3i", "int x", Vector3<int>, x);
    REGISTER_PROPERTY("Vector3i", "int y", Vector3<int>, y);
    REGISTER_PROPERTY("Vector3i", "int z", Vector3<int>, z);
    REGISTER_METHOD("Vector3i", "float Length()", Vector3<int>, Length);
});
