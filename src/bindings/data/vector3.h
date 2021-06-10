#pragma once
#pragma warning(disable : 4244)  // Disable warnings about loss of data in type conversion
#include "Log.h"
#include "../../helpers/module.h"

using namespace Helpers;

namespace Data
{
    class Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3(int x, int y, int z) : x(x), y(y), z(z){};

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

        float Distance(Vector3& other)
        {
            return (float)sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
        }

        std::string ToString()
        {
            std::stringstream str;
            str << "Vector3{ x: " << x << ", y: " << y << ", z: " << z << " }";
            return str.str();
        }

        template<typename T>
        static void Construct(void* memory, T x, T y, T z)
        {
            new(memory) Vector3(x, y, z);
        }

        template<typename T>
        static void ConstructFromList(void* memory, T* values)
        {
            new(memory) Vector3(values[0], values[1], values[2]);
        }
    };
}  // namespace Data
