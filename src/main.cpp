#include <raylib-cpp.hpp>

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
        textColor.DrawText("Congrats! You created your first window!", 190, 200, 20);
        EndDrawing();
    }

    UnloadSound(bgMusic);
    CloseAudioDevice();

    return 0;
}