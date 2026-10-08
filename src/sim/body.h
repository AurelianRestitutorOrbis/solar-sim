#pragma once
#include "sim/vec2.h"
#include <string>

struct Body {
    std::string name;
    double mass = 0.0;
    Vec2 position; // AU
    Vec2 velocity; // AU/year
    Vec2 acceleration; // AU/year^2

};