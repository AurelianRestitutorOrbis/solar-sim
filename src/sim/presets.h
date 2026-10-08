#pragma once
#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include "sim/body.h"

inline constexpr double earthMassInSolarMasses = 3.003e-6; // Earth mass in solar masses

struct Preset {
    const char* name;
    double earthMasses;
    double earthRadii;
    std::uint32_t color; // 0xRRGGBBAA
};

inline constexpr std::array<Preset, 11> presets = {{
    {"Sun",     332946.0, 109.2,  0xFDF900FF},
    {"Mercury", 0.0553,   0.383,  0x9A9A9AFF},
    {"Venus",   0.815,    0.950,  0xE8B36BFF},
    {"Earth",   1.0,      1.0,    0x2F7FE8FF},
    {"Mars",    0.107,    0.532,  0xC8553DFF},
    {"Jupiter", 317.8,    10.97,  0xD9A066FF},
    {"Saturn",  95.2,     9.14,   0xE6CF8BFF},
    {"Uranus",  14.5,     3.98,   0x8FD9E0FF},
    {"Neptune", 17.1,     3.86,   0x4063D8FF},
    {"Moon",    0.0123,   0.273,  0xC8C8C8FF},
    {"Ceres",   0.00016,  0.074,  0x8C7B6BFF},
}};

inline double displayRadiusAU(double earthRadii) {
    constexpr double earthRadiusAU = 0.016;
    return earthRadiusAU * std::pow(earthRadii, 0.4); // Non-linear scaling for better visibility

}

inline Body makeBody(const std::string& name, double earthMasses, double earthRadii,
                    std::uint32_t color, Vec2 position, Vec2 velocity) {
    return {name,
            earthMasses * earthMassInSolarMasses,
            displayRadiusAU(earthRadii),
            color,
            position,
            velocity};
}

inline Body makeBody(const Preset& preset, Vec2 position, Vec2 velocity) {
    return makeBody(preset.name, preset.earthMasses, preset.earthRadii, 
                    preset.color, position, velocity);
}