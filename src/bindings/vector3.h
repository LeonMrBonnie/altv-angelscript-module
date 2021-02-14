#pragma once
#include "Log.h"
#include "../helpers/module.h"

using namespace Helpers;

namespace Helpers
{
    class Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z) : x(x), y(y), z(z) {};

        float Length()
        {
            return sqrt(x * x + y * y + z * z);
        }

        Vector3 AddVector(Vector3& other)
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        Vector3 AddValues(float _x, float _y, float _z)
        {
            return Vector3(x + _x, y + _y, z + _z);
        }
        Vector3 AddValue(float value)
        {
            return Vector3(x + value, y + value, z + value);
        }

        Vector3 SubVector(Vector3& other)
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        Vector3 SubValues(float _x, float _y, float _z)
        {
            return Vector3(x - _x, y - _y, z - _z);
        }
        Vector3 SubValue(float value)
        {
            return Vector3(x - value, y - value, z - value);
        }

        Vector3 MultVector(Vector3& other)
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }
        Vector3 MultValues(float _x, float _y, float _z)
        {
            return Vector3(x * _x, y * _y, z * _z);
        }
        Vector3 MultValue(float value)
        {
            return Vector3(x * value, y * value, z * value);
        }

        std::string ToString()
        {
            std::stringstream str;
            str << "Vector3{ x: " << x << ", y: " << y << ", z: " << z << " }";
            return str.str();
        }

        static void Construct(float x, float y, float z, void* memory)
        {
            new(memory) Vector3(x, y, z);
        }
    };

    static void RegisterVector3(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("Vector3", Vector3, asOBJ_VALUE | asOBJ_POD, "Three-dimensional vector");
        REGISTER_CONSTRUCTOR("Vector3", "float x, float y, float z", Vector3::Construct);
        REGISTER_PROPERTY("Vector3", "float x", Vector3, x);
        REGISTER_PROPERTY("Vector3", "float y", Vector3, y);
        REGISTER_PROPERTY("Vector3", "float z", Vector3, z);
        REGISTER_METHOD("Vector3", "float Length()", Vector3, Length);
        REGISTER_METHOD("Vector3", "Vector3 Add(Vector3 vector)", Vector3, AddVector);
        REGISTER_METHOD("Vector3", "Vector3 Add(float x, float y, float z)", Vector3, AddValues);
        REGISTER_METHOD("Vector3", "Vector3 Add(float value)", Vector3, AddValue);
        REGISTER_METHOD("Vector3", "Vector3 Sub(Vector3 vector)", Vector3, SubVector);
        REGISTER_METHOD("Vector3", "Vector3 Sub(float x, float y, float z)", Vector3, SubValues);
        REGISTER_METHOD("Vector3", "Vector3 Sub(float value)", Vector3, SubValue);
        REGISTER_METHOD("Vector3", "Vector3 Mult(Vector3 vector)", Vector3, MultVector);
        REGISTER_METHOD("Vector3", "Vector3 Mult(float x, float y, float z)", Vector3, MultValues);
        REGISTER_METHOD("Vector3", "Vector3 Mult(float value)", Vector3, MultValue);
        // Implicit conversion to string
        REGISTER_METHOD("Vector3", "string opImplConv() const", Vector3, ToString);
    }
}
