#include <raylib-cpp.hpp>
#include <vector>
#include "headers/game.h"


int main() {
    
    // Initialization
    int screenWidth = 1600;
    int screenHeight = 900;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");

    InitAudioDevice();

    Sound bgMusic = LoadSound("assets/audio/chipi.wav"); 
    PlaySound(bgMusic);
    
    SetTargetFPS(60);

        

    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // Update

        // TODO: Update your variables here

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    UnloadSound(bgMusic);
    CloseAudioDevice();

    return 0;
}