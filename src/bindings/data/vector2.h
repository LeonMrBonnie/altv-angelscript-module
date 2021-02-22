#pragma once
#pragma warning(disable : 4244) // Disable warnings about loss of data in type conversion
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

namespace Helpers
{
    class Vector2
    {
    public:
        float x;
        float y;

        Vector2(float x, float y) : x(x), y(y) {}
        Vector2(int x, int y) : x(x), y(y) {}

        float Length()
        {
            return sqrt(x * x + y * y);
        }

        Vector2 AddVector(Vector2& other)
        {
            return Vector2(x + other.x, y + other.y);
        }
        Vector2 AddValues(float _x, float _y)
        {
            return Vector2(x + _x, y + _y);
        }
        Vector2 AddValue(float value)
        {
            return Vector2(x + value, y + value);
        }

        Vector2 SubVector(Vector2& other)
        {
            return Vector2(x - other.x, y - other.y);
        }
        Vector2 SubValues(float _x, float _y)
        {
            return Vector2(x - _x, y - _y);
        }
        Vector2 SubValue(float value)
        {
            return Vector2(x - value, y - value);
        }

        Vector2 MultVector(Vector2& other)
        {
            return Vector2(x * other.x, y * other.y);
        }
        Vector2 MultValues(float _x, float _y)
        {
            return Vector2(x * _x, y * _y);
        }
        Vector2 MultValue(float value)
        {
            return Vector2(x * value, y * value);
        }

        std::string ToString()
        {
            std::stringstream str;
            str << "Vector2{ x: " << x << ", y: " << y << " }";
            return str.str();
        }

        template<typename T>
        static void Construct(void* memory, T x, T y)
        {
            new(memory) Vector2(x, y);
        }
    };
}

namespace Data
{
    static void RegisterVector2(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("Vector2", Vector2, asOBJ_VALUE | asOBJ_POD, "Two-dimensional vector");
        REGISTER_CONSTRUCTOR("Vector2", "float x, float y", Vector2::Construct<float>);
        REGISTER_CONSTRUCTOR("Vector2", "int x, int y", Vector2::Construct<int>);
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
        // Implicit conversion to string
        REGISTER_METHOD("Vector2", "string opImplConv() const", Vector2, ToString);
    }
}
