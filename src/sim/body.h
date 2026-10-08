#pragma once
#include <cstdint>
#include <deque>
#include <string>
#include "sim/vec2.h"

struct Body {
    std::string name;
    double mass = 0.0;                 // solar masses
    double radius = 0.0;               // AU, for drawing and collisions
    std::uint32_t color = 0xFFFFFFFF;  // 0xRRGGBBAA
    Vec2 position;                     // AU
    Vec2 velocity;                     // AU/year
    Vec2 acceleration;                 // AU/year^2
    std::deque<Vec2> trail;            // recent positions, oldest first
};