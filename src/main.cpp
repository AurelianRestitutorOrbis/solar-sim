#include "raylib.h"

#include <cmath>
#include <iostream>
#include "sim/body.h"
#include "sim/vec2.h"
#include "sim/simulation.h"
#include <deque>

double circularSpeed(double r) {
    return std::sqrt(G * 1.0 / r); // Assuming mass of the central body (Sun) is 1 solar mass
}

const double pixelsPerAU = 250.0; // Scale factor for rendering


Vector2 toScreen(Vec2 p) {
    return {static_cast<float>(GetScreenWidth() / 2 + p.x * pixelsPerAU),
            static_cast<float>(GetScreenHeight() / 2 - p.y * pixelsPerAU)};
}

int main() {

   
    // Initialization

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "Solar System Simulation");
    SetTargetFPS(60);

   std::vector<Body> bodies = {
    {"Sun",     1.0,     0.080, 0xFDF900FF, {0.0,   0.0}, {0.0, 0.0}},
    {"Mercury", 1.66e-7, 0.020, 0x828282FF, {0.387, 0.0}, {0.0, circularSpeed(0.387)}},
    {"Venus",   2.45e-6, 0.028, 0xFFA100FF, {0.723, 0.0}, {0.0, circularSpeed(0.723)}},
    {"Earth",   3.00e-6, 0.040, 0x0079F1FF, {1.0,   0.0}, {0.0, circularSpeed(1.0)}},
};

    const std::size_t maxTrailPoints = 150; // Maximum number of points in the trail

    const double dt = 0.0005; // Time step in years
    const int stepsPerFrame = 5;

    computeAccelerations(bodies);

    // Main game loop
    while (!WindowShouldClose()) {
        for (int i = 0; i < stepsPerFrame; ++i) {
            step(bodies, dt);
            removeCollisions(bodies);
        }

        for (Body& body : bodies) {
            body.trail.push_back(body.position);
            if (body.trail.size() > maxTrailPoints) {
                body.trail.pop_front();
                }
            }


        BeginDrawing();
        ClearBackground(BLACK);

        for (const Body& body : bodies) {
            const Color color = GetColor(body.color);
            for (std::size_t k = 1; k < body.trail.size(); ++k) {
                const float alpha = static_cast<float>(k) / body.trail.size();
                DrawLineV(toScreen(body.trail[k - 1]), toScreen(body.trail[k]), Fade(color, alpha));
                }
            }

for (const Body& body : bodies) {
    DrawCircleV(toScreen(body.position),
                static_cast<float>(body.radius * pixelsPerAU),
                GetColor(body.color));
}

        EndDrawing();
    }

    CloseWindow();
    return 0;
}