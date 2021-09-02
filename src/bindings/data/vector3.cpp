#include "vector3.h"

static DataExtension vector3Extension([](asIScriptEngine* engine, DocsGenerator* docs) {
    using namespace Data;
    REGISTER_VALUE_CLASS("Vector3", Vector3, asOBJ_VALUE | asOBJ_POD, "Three-dimensional vector");
    REGISTER_CONSTRUCTOR("Vector3", "float x, float y, float z", Vector3::Construct<float>);
    REGISTER_CONSTRUCTOR("Vector3", "int x, int y, int z", Vector3::Construct<int>);
    REGISTER_INIT_LIST_CONSTRUCTOR("Vector3", "float, float, float", Vector3::ConstructFromList<float>);
    // REGISTER_INIT_LIST_CONSTRUCTOR("Vector3", "int, int, int", Vector3::ConstructFromList<int>);
    REGISTER_PROPERTY("Vector3", "const float x", Vector3, x);
    REGISTER_PROPERTY("Vector3", "const float y", Vector3, y);
    REGISTER_PROPERTY("Vector3", "const float z", Vector3, z);
    REGISTER_METHOD("Vector3", "Vector3 Add(Vector3 vector)", Vector3, AddVector);
    REGISTER_METHOD("Vector3", "Vector3 Add(float x, float y, float z)", Vector3, AddValues);
    REGISTER_METHOD("Vector3", "Vector3 Add(float value)", Vector3, AddValue);
    REGISTER_METHOD("Vector3", "Vector3 Sub(Vector3 vector)", Vector3, SubVector);
    REGISTER_METHOD("Vector3", "Vector3 Sub(float x, float y, float z)", Vector3, SubValues);
    REGISTER_METHOD("Vector3", "Vector3 Sub(float value)", Vector3, SubValue);
    REGISTER_METHOD("Vector3", "Vector3 Mult(Vector3 vector)", Vector3, MultVector);
    REGISTER_METHOD("Vector3", "Vector3 Mult(float x, float y, float z)", Vector3, MultValues);
    REGISTER_METHOD("Vector3", "Vector3 Mult(float value)", Vector3, MultValue);
    REGISTER_METHOD("Vector3", "float Distance(Vector3 vector)", Vector3, Distance);
    REGISTER_METHOD("Vector3", "float Length()", Vector3, Length);
    REGISTER_METHOD("Vector3", "Vector3 ToRadians()", Vector3, ToRadians);
    REGISTER_METHOD("Vector3", "Vector3 ToDegrees()", Vector3, ToDegrees);

    // Implicit conversion to string
    REGISTER_METHOD("Vector3", "string opImplConv() const", Vector3, ToString);
});
