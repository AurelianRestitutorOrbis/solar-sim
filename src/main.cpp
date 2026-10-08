#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <cmath>
#include <iostream>
#include "sim/body.h"
#include "sim/vec2.h"
#include "sim/simulation.h"
#include <deque>
#include <algorithm>
#include "sim/presets.h"

double circularSpeed(double r) {
    return std::sqrt(G * 1.0 / r); // Assuming mass of the central body (Sun) is 1 solar mass
}

const double pixelsPerAU = 250.0; // Scale factor for rendering
constexpr int panelWidth = 280; // Width of the side panel in pixels

Vector2 viewCentre() {
    return {(GetScreenWidth() - panelWidth) / 2.0f, GetScreenHeight() / 2.0f};
}

Vector2 toScreen(Vec2 p) {
    const Vector2 c = viewCentre();
    return {static_cast<float>(c.x + p.x * pixelsPerAU),
            static_cast<float>(c.y - p.y * pixelsPerAU)};
}

Vec2 toWorld(Vector2 s) {
    const Vector2 c = viewCentre();
    return {(s.x - c.x) / pixelsPerAU,
            (c.y - s.y) / pixelsPerAU};
}

float drawRadius(const Body& body) {
    return std::max(2.0f, static_cast<float>(body.radius * pixelsPerAU));
}



   void applyDarkTheme() {
       const auto set = [](int property, unsigned int rgba) {
           GuiSetStyle(DEFAULT, property, static_cast<int>(rgba));
       };
       set(BACKGROUND_COLOR,      0x15171CFF);
       set(LINE_COLOR,            0x3A3F4BFF);
       set(BORDER_COLOR_NORMAL,   0x3A3F4BFF);
       set(BASE_COLOR_NORMAL,     0x22252DFF);
       set(TEXT_COLOR_NORMAL,     0xC9CED8FF);
       set(BORDER_COLOR_FOCUSED,  0x6C8CD5FF);
       set(BASE_COLOR_FOCUSED,    0x2C313CFF);
       set(TEXT_COLOR_FOCUSED,    0xFFFFFFFF);
       set(BORDER_COLOR_PRESSED,  0x8FB0FFFF);
       set(BASE_COLOR_PRESSED,    0x34509AFF);
       set(TEXT_COLOR_PRESSED,    0xFFFFFFFF);
       set(BORDER_COLOR_DISABLED, 0x2A2D35FF);
       set(BASE_COLOR_DISABLED,   0x1B1D23FF);
       set(TEXT_COLOR_DISABLED,   0x5A5F6BFF);
   }

      void drawPanel(bool& paused) {
       const float x = static_cast<float>(GetScreenWidth() - panelWidth);
       const float left = x + 16.0f;
       const float width = panelWidth - 32.0f;

       DrawRectangle(static_cast<int>(x), 0, panelWidth, GetScreenHeight(),
                     GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
       DrawLine(static_cast<int>(x), 0, static_cast<int>(x), GetScreenHeight(),
                GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

       if (GuiButton({left, 700, width, 32}, paused ? "Resume (Space)" : "Pause (Space)")) {
           paused = !paused;
       }

       GuiLabel({left, 746, width, 20}, "Left-drag: place and aim");
       GuiLabel({left, 766, width, 20}, "Right-click: cancel");
   }


int main() {

   
    // Initialization

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1600, 900, "Solar System Simulation");
    SetTargetFPS(60);

    SetWindowMinSize(900, 800);
    applyDarkTheme();

      std::vector<Body> bodies = {
       makeBody(presets[0], {0.0,   0.0}, {0.0, 0.0}),
       makeBody(presets[1], {0.387, 0.0}, {0.0, circularSpeed(0.387)}),
       makeBody(presets[2], {0.723, 0.0}, {0.0, circularSpeed(0.723)}),
       makeBody(presets[3], {1.0,   0.0}, {0.0, circularSpeed(1.0)}),
   };

    const std::size_t maxTrailPoints = 150; // Maximum number of points in the trail

    const double dt = 0.0005; // Time step in years
    const int stepsPerFrame = 5;

    const double velocityPerAU = 10.0; // AU/year of speed for placing new bodies
    const int predictionSteps = 5000; // Number of steps to predict

    bool placing = false; bool paused = false;
    Body candidate;

    computeAccelerations(bodies);

    // Main game loop

    while (!WindowShouldClose()) {

           if (IsKeyPressed(KEY_SPACE)) {
       paused = !paused;
   }

        const Vector2 mouse = GetMousePosition(); // Mouse position in screen coordinates
        const Vec2 mouseWorld = toWorld(mouse);
        const bool overPanel = mouse.x >= GetScreenWidth() - panelWidth;


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !overPanel) { // Start placing a new body
            placing = true;
            candidate = makeBody(presets[3], mouseWorld, {0.0, 0.0}); // Default to Earth-like body
        }

        if (placing) { // Update candidate's velocity based on mouse position
            candidate.velocity = (mouseWorld - candidate.position) * velocityPerAU;

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                placing = false;
            } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                bodies.push_back(candidate);
                computeAccelerations(bodies);
                placing = false;
            }
        }

        if (!paused) {
            for (int i = 0; i < stepsPerFrame; ++i) {
                step(bodies, dt);
                removeCollisions(bodies);
            }
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
            for (std::size_t k = 1; k < body.trail.size(); ++k) { // Draw the trail with fading effect
                const float alpha = static_cast<float>(k) / body.trail.size();
                DrawLineV(toScreen(body.trail[k - 1]), toScreen(body.trail[k]), Fade(color, alpha));
                }
            }

        for (const Body& body : bodies) { 
            DrawCircleV(toScreen(body.position), // Draw the body as a circle
                drawRadius(body),
                GetColor(body.color));
            }

        if (paused) {
            DrawText("PAUSED", 20, 20, 30, RAYWHITE);
        }

        if (placing) {
            const std::vector<Vec2> path = predictPath(bodies, candidate, dt, predictionSteps);
            for (std::size_t k = 1; k < path.size(); ++k) {
                DrawLineV(toScreen(path[k - 1]), toScreen(path[k]), Fade(WHITE, 0.6f));
            }

            DrawLineV(toScreen(candidate.position), GetMousePosition(), GREEN);
            DrawCircleV(toScreen(candidate.position),
            drawRadius(candidate),
            Fade(GetColor(candidate.color), 0.7f));
        }

        if (placing) GuiLock(); else GuiUnlock();
        drawPanel(paused);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}