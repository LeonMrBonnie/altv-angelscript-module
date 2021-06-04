#pragma once
#pragma warning(disable : 4244)  // Disable warnings about loss of data in type conversion
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

namespace Data
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

        float Distance(Vector2& other)
        {
            return (float)sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
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
}  // namespace Data
