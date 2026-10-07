#include "raylib.h"

int main() {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Solar System Simulation");

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(screenWidth / 2, screenHeight / 2, 20.0f, YELLOW); // Sun
        EndDrawing();
    }

    CloseWindow();
    return 0;
}