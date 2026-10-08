#pragma once
#include <numbers>
#include <vector>
#include "sim/body.h"
#include <math.h>
void removeCollisions(std::vector<Body>& bodies);
constexpr double pi = 3.141592653589793238462643383279;
// Units: AU, solar masses, years. In these units, G = 4 * pi^2.
inline constexpr double G = 4.0 * (pi * pi);

void computeAccelerations(std::vector<Body>& bodies);
void step(std::vector<Body>& bodies, double dt);