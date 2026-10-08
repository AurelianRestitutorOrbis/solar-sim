#include "raylib.h"

#include <cmath>
#include <iostream>
#include "sim/body.h"
#include "sim/vec2.h"
#include "sim/simulation.h"

double circularSpeed(double r) {
    return std::sqrt(G * 1.0 / r); // Assuming mass of the central body (Sun) is 1 solar mass
}

int main() {

   
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Solar System Simulation");
    SetTargetFPS(60);

    std::vector<Body> bodies = {
        {"Sun", 1.0, {0.0, 0.0}, {0.0, 0.0}},
        {"Mercury",1.66e-7, {0.387, 0.0}, {0.0, circularSpeed(0.387)}},
        {"Venus", 2.45e-6, {0.723, 0.0}, {0.0, circularSpeed(0.723)}},
        {"Earth", 3.00e-6, {1.0, 0.0}, {0.0, circularSpeed(1.0)}}
    };
    const std::vector<float> radii = {20.0f, 5.0f, 7.0f, 10.0f}; // Radii for Sun, Mercury, Venus, Earth
    const std::vector<Color> colors = {YELLOW, GRAY, ORANGE, BLUE}; // Colors for Sun, Mercury, Venus, Earth

    const double dt = 0.0005; // Time step in years
    const int stepsPerFrame = 5;
    const double pixelsPerAU = 250.0; // Scale factor for rendering

    computeAccelerations(bodies);

    // Main game loop
    while (!WindowShouldClose()) {
        for (int i = 0; i < stepsPerFrame; ++i) {
            step(bodies, dt);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for (std::size_t i = 0; i < bodies.size(); ++i) {
            const int x = static_cast<int>(screenWidth / 2 + bodies[i].position.x * pixelsPerAU);
            const int y = static_cast<int>(screenHeight / 2 - bodies[i].position.y * pixelsPerAU);
            DrawCircle(x, y, radii[i], colors[i]);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}