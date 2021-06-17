#include "vector2.h"

static DataExtension vector2Extension([](asIScriptEngine* engine, DocsGenerator* docs) {
    using namespace Data;
    REGISTER_VALUE_CLASS("Vector2", Vector2, asOBJ_VALUE | asOBJ_POD, "Two-dimensional vector");
    REGISTER_CONSTRUCTOR("Vector2", "float x, float y", Vector2::Construct<float>);
    REGISTER_CONSTRUCTOR("Vector2", "int x, int y", Vector2::Construct<int>);
    REGISTER_INIT_LIST_CONSTRUCTOR("Vector2", "float, float", Vector2::ConstructFromList<float>);
    // REGISTER_INIT_LIST_CONSTRUCTOR("Vector2", "int, int", Vector2::ConstructFromList<int>);
    REGISTER_PROPERTY("Vector2", "float x", Vector2, x);
    REGISTER_PROPERTY("Vector2", "float y", Vector2, y);
    REGISTER_METHOD("Vector2", "float Length()", Vector2, Length);
    REGISTER_METHOD("Vector2", "Vector2 Add(Vector2 vector)", Vector2, AddVector);
    REGISTER_METHOD("Vector2", "Vector2 Add(float x, float y)", Vector2, AddValues);
    REGISTER_METHOD("Vector2", "Vector2 Add(float value)", Vector2, AddValue);
    REGISTER_METHOD("Vector2", "Vector2 Sub(Vector2 vector)", Vector2, SubVector);
    REGISTER_METHOD("Vector2", "Vector2 Sub(float x, float y)", Vector2, SubValues);
    REGISTER_METHOD("Vector2", "Vector2 Sub(float value)", Vector2, SubValue);
    REGISTER_METHOD("Vector2", "Vector2 Mult(Vector2 vector)", Vector2, MultVector);
    REGISTER_METHOD("Vector2", "Vector2 Mult(float x, float y)", Vector2, MultValues);
    REGISTER_METHOD("Vector2", "Vector2 Mult(float value)", Vector2, MultValue);
    REGISTER_METHOD("Vector2", "float Distance(Vector2 vector)", Vector2, Distance);
    // Implicit conversion to string
    REGISTER_METHOD("Vector2", "string opImplConv() const", Vector2, ToString);
});
