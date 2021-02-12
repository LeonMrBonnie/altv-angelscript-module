#pragma once
#include "Log.h"
#include "../helpers/module.h"

using namespace Helpers;

namespace Helpers
{
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

        Vector3<T> AddVector(Vector3<T> other)
        {
            return Vector3<T>(x + other.x, y + other.y, z + other.z);
        }
        Vector3<T> AddValues(T _x, T _y, T _z)
        {
            return Vector3<T>(x + _x, y + _y, z + _z);
        }
        Vector3<T> AddValue(T value)
        {
            return Vector3<T>(x + value, y + value, z + value);
        }

        Vector3<T> SubVector(Vector3<T> other)
        {
            return Vector3<T>(x - other.x, y - other.y, z - other.z);
        }
        Vector3<T> SubValues(T _x, T _y, T _z)
        {
            return Vector3<T>(x - _x, y - _y, z - _z);
        }
        Vector3<T> SubValue(T value)
        {
            return Vector3<T>(x - value, y - value, z - value);
        }

        Vector3<T> MultVector(Vector3<T> other)
        {
            return Vector3<T>(x * other.x, y * other.y, z * other.z);
        }
        Vector3<T> MultValues(T _x, T _y, T _z)
        {
            return Vector3<T>(x * _x, y * _y, z * _z);
        }
        Vector3<T> MultValue(T value)
        {
            return Vector3<T>(x * value, y * value, z * value);
        }

        std::string ToString()
        {
            std::stringstream str;
            str << "Vector3{ x: " << x << ", y: " << y << ", z: " << z << " }";
            return str.str();
        }

        static void Construct(T x, T y, T z, void* memory)
        {
            new(memory) Vector3<T>(x, y, z);
        }
    };

    static void RegisterVector3(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("Vector3f", Vector3<float>, asOBJ_VALUE | asOBJ_POD, "Three-dimensional float vector");
        REGISTER_CONSTRUCTOR("Vector3f", "float x, float y, float z", Vector3<float>::Construct);
        REGISTER_PROPERTY("Vector3f", "float x", Vector3<float>, x);
        REGISTER_PROPERTY("Vector3f", "float y", Vector3<float>, y);
        REGISTER_PROPERTY("Vector3f", "float z", Vector3<float>, z);
        REGISTER_METHOD("Vector3f", "float Length()", Vector3<float>, Length);
        REGISTER_METHOD("Vector3f", "Vector3f Add(Vector3f vector)", Vector3<float>, AddVector);
        REGISTER_METHOD("Vector3f", "Vector3f Add(float x, float y, float z)", Vector3<float>, AddValues);
        REGISTER_METHOD("Vector3f", "Vector3f Add(float value)", Vector3<float>, AddValue);
        REGISTER_METHOD("Vector3f", "Vector3f Sub(Vector3f vector)", Vector3<float>, SubVector);
        REGISTER_METHOD("Vector3f", "Vector3f Sub(float x, float y, float z)", Vector3<float>, SubValues);
        REGISTER_METHOD("Vector3f", "Vector3f Sub(float value)", Vector3<float>, SubValue);
        REGISTER_METHOD("Vector3f", "Vector3f Mult(Vector3f vector)", Vector3<float>, MultVector);
        REGISTER_METHOD("Vector3f", "Vector3f Mult(float x, float y, float z)", Vector3<float>, MultValues);
        REGISTER_METHOD("Vector3f", "Vector3f Mult(float value)", Vector3<float>, MultValue);
        // Implicit conversion to string
        REGISTER_METHOD("Vector3f", "string opImplConv() const", Vector3<float>, ToString);

        REGISTER_VALUE_CLASS("Vector3i", Vector3<int>, asOBJ_VALUE | asOBJ_POD, "Three-dimensional integer vector");
        REGISTER_CONSTRUCTOR("Vector3i", "int x, int y, int z", Vector3<int>::Construct);
        REGISTER_PROPERTY("Vector3i", "int x", Vector3<int>, x);
        REGISTER_PROPERTY("Vector3i", "int y", Vector3<int>, y);
        REGISTER_PROPERTY("Vector3i", "int z", Vector3<int>, z);
        REGISTER_METHOD("Vector3i", "float Length()", Vector3<int>, Length);
        REGISTER_METHOD("Vector3i", "Vector3i Add(Vector3i vector)", Vector3<int>, AddVector);
        REGISTER_METHOD("Vector3i", "Vector3i Add(int x, int y, int z)", Vector3<int>, AddValues);
        REGISTER_METHOD("Vector3i", "Vector3i Add(int value)", Vector3<int>, AddValue);
        REGISTER_METHOD("Vector3i", "Vector3i Sub(Vector3i vector)", Vector3<int>, SubVector);
        REGISTER_METHOD("Vector3i", "Vector3i Sub(int x, int y, int z)", Vector3<int>, SubValues);
        REGISTER_METHOD("Vector3i", "Vector3i Sub(int value)", Vector3<int>, SubValue);
        REGISTER_METHOD("Vector3i", "Vector3i Mult(Vector3i vector)", Vector3<int>, MultVector);
        REGISTER_METHOD("Vector3i", "Vector3i Mult(int x, int y, int z)", Vector3<int>, MultValues);
        REGISTER_METHOD("Vector3i", "Vector3i Mult(int value)", Vector3<int>, MultValue);
        // Implicit conversion to string
        REGISTER_METHOD("Vector3i", "string opImplConv() const", Vector3<int>, ToString);
    }
}
