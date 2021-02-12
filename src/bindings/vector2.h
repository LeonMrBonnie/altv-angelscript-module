#pragma once
#include "Log.h"
#include "../helpers/module.h"

using namespace Helpers;

namespace Helpers
{
    template<typename T>
    class Vector2
    {
    public:
        T x;
        T y;

        Vector2(T x, T y) : x(x), y(y) {};

        float Length()
        {
            return sqrt(x * x + y * y);
        }

        Vector2<T> AddVector(Vector2<T> other)
        {
            return Vector2<T>(x + other.x, y + other.y);
        }
        Vector2<T> AddValues(T _x, T _y)
        {
            return Vector2<T>(x + _x, y + _y);
        }
        Vector2<T> AddValue(T value)
        {
            return Vector2<T>(x + value, y + value);
        }

        Vector2<T> SubVector(Vector2<T> other)
        {
            return Vector2<T>(x - other.x, y - other.y);
        }
        Vector2<T> SubValues(T _x, T _y)
        {
            return Vector2<T>(x - _x, y - _y);
        }
        Vector2<T> SubValue(T value)
        {
            return Vector2<T>(x - value, y - value);
        }

        Vector2<T> MultVector(Vector2<T> other)
        {
            return Vector2<T>(x * other.x, y * other.y);
        }
        Vector2<T> MultValues(T _x, T _y)
        {
            return Vector2<T>(x * _x, y * _y);
        }
        Vector2<T> MultValue(T value)
        {
            return Vector2<T>(x * value, y * value);
        }

        static void Construct(T x, T y, void* memory)
        {
            new(memory) Vector2<T>(x, y);
        }
    };

    static void RegisterVector2(asIScriptEngine* engine, DocsGenerator* docs)
    {
        REGISTER_VALUE_CLASS("Vector2f", Vector2<float>, asOBJ_VALUE | asOBJ_POD, "Two-dimensional float vector");
        REGISTER_CONSTRUCTOR("Vector2f", "float x, float y", Vector2<float>::Construct);
        REGISTER_PROPERTY("Vector2f", "float x", Vector2<float>, x);
        REGISTER_PROPERTY("Vector2f", "float y", Vector2<float>, y);
        REGISTER_METHOD("Vector2f", "float Length()", Vector2<float>, Length);
        REGISTER_METHOD("Vector2f", "Vector2f Add(Vector2f vector)", Vector2<float>, AddVector);
        REGISTER_METHOD("Vector2f", "Vector2f Add(float x, float y)", Vector2<float>, AddValues);
        REGISTER_METHOD("Vector2f", "Vector2f Add(float value)", Vector2<float>, AddValue);
        REGISTER_METHOD("Vector2f", "Vector2f Sub(Vector2f vector)", Vector2<float>, SubVector);
        REGISTER_METHOD("Vector2f", "Vector2f Sub(float x, float y)", Vector2<float>, SubValues);
        REGISTER_METHOD("Vector2f", "Vector2f Sub(float value)", Vector2<float>, SubValue);
        REGISTER_METHOD("Vector2f", "Vector2f Mult(Vector2f vector)", Vector2<float>, MultVector);
        REGISTER_METHOD("Vector2f", "Vector2f Mult(float x, float y)", Vector2<float>, MultValues);
        REGISTER_METHOD("Vector2f", "Vector2f Mult(float value)", Vector2<float>, MultValue);

        REGISTER_VALUE_CLASS("Vector2i", Vector2<int>, asOBJ_VALUE | asOBJ_POD, "Two-dimensional integer vector");
        REGISTER_CONSTRUCTOR("Vector2i", "int x, int y, int z", Vector2<int>::Construct);
        REGISTER_PROPERTY("Vector2i", "int x", Vector2<int>, x);
        REGISTER_PROPERTY("Vector2i", "int y", Vector2<int>, y);
        REGISTER_METHOD("Vector2i", "float Length()", Vector2<int>, Length);
        REGISTER_METHOD("Vector2i", "Vector2i Add(Vector2i vector)", Vector2<int>, AddVector);
        REGISTER_METHOD("Vector2i", "Vector2i Add(int x, int y)", Vector2<int>, AddValues);
        REGISTER_METHOD("Vector2i", "Vector2i Add(int value)", Vector2<int>, AddValue);
        REGISTER_METHOD("Vector2i", "Vector2i Sub(Vector2i vector)", Vector2<int>, SubVector);
        REGISTER_METHOD("Vector2i", "Vector2i Sub(int x, int y)", Vector2<int>, SubValues);
        REGISTER_METHOD("Vector2i", "Vector2i Sub(int value)", Vector2<int>, SubValue);
        REGISTER_METHOD("Vector2i", "Vector2i Mult(Vector2i vector)", Vector2<int>, MultVector);
        REGISTER_METHOD("Vector2i", "Vector2i Mult(int x, int y)", Vector2<int>, MultValues);
        REGISTER_METHOD("Vector2i", "Vector2i Mult(int value)", Vector2<int>, MultValue);
    }
}
