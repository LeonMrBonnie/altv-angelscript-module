#pragma once
#include "../../helpers/module.h"

namespace Data
{
    struct VehicleNeon
    {
        VehicleNeon(bool front, bool back, bool left, bool right) : front(front), back(back), left(left), right(right) {}

        bool front;
        bool back;
        bool left;
        bool right;
    };
}  // namespace Data
