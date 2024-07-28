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
    
    Camera2D camera = { 0 };
    camera.target = { screenWidth / 2.0f,screenHeight / 2.0f }; // Jest to tak ustawione aby był kwadrat 0,0 w górnym lewym rogu 
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // Update
        // TODO: Update your variables here

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                Shop build = Shop();
                build.drawBuilding(0,0);
                Home build1 = Home();
                build1.drawBuilding(1,0);
                
            EndMode2D();
        EndDrawing();
    }

    UnloadSound(bgMusic);
    CloseAudioDevice();

    return 0;
}
