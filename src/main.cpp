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
        {"Sun", 1.0, {0.0, 0.0}, {0.0, 0.0}},
        {"Mercury",1.66e-7, {0.387, 0.0}, {0.0, circularSpeed(0.387)}},
        {"Venus", 2.45e-6, {0.723, 0.0}, {0.0, circularSpeed(0.723)}},
        {"Earth", 3.00e-6, {1.0, 0.0}, {0.0, circularSpeed(1.0)}}
    };
    const std::vector<float> radii = {20.0f, 5.0f, 7.0f, 10.0f}; // Radii for Sun, Mercury, Venus, Earth
    const std::vector<Color> colors = {YELLOW, GRAY, ORANGE, BLUE}; // Colors for Sun, Mercury, Venus, Earth
    const std::size_t maxTrailPoints = 150; // Maximum number of points in the trail
    std::vector<std::deque<Vec2>> trails(bodies.size()); // Trails for each body

    const double dt = 0.0005; // Time step in years
    const int stepsPerFrame = 5;

    computeAccelerations(bodies);

    // Main game loop
    while (!WindowShouldClose()) {
        for (int i = 0; i < stepsPerFrame; ++i) {
            step(bodies, dt);
        }

        for (std::size_t i = 0; i < bodies.size(); ++i) {
            trails[i].push_back(bodies[i].position);
            if (trails[i].size() > maxTrailPoints) {
                trails[i].pop_front();
            }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        for (std::size_t i = 0; i < bodies.size(); ++i) { 
            for (std::size_t k = 1; k < trails[i].size(); ++k) {
                const float alpha = static_cast<float>(k) / trails[i].size(); // Fade effect for the trail
                DrawLineV(toScreen(trails[i][k - 1]), toScreen(trails[i][k]), Fade(colors[i], alpha)); // Draw trail with fading effect
            }   
        }

        for (std::size_t i = 0; i < bodies.size(); ++i) { // Draw the bodies
            DrawCircleV(toScreen(bodies[i].position), radii[i], colors[i]); //
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}