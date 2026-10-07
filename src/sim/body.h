#pragma once
#include "sim/vec2.h"
#include <string>

struct Body {
    std::string name;
    Vec2 position; // AU
    Vec2 velocity; // AU/day
    Vec2 acceleration; // AU/day^2
    double mass = 0.0;
};