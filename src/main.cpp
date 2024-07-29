#include <raylib-cpp.hpp>
#include <vector>
#include "headers/game.h"
using namespace std;
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

    size_t rows = 20;
    size_t columns = 15;
    vector<GameCell> map;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            map.push_back(GameCell(i,j));
        }   
    }
    map[10].setHome();
    map[11].setShop();
    map[12].setRoad(0b0001);
    map[13].setRoad(0b0011);
    map[14].setRoad(0b0111);
    map[15].setRoad(0b1111);
   
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

                for (auto &&i : map)
                {
                    i.drawCell();
                }
                
            EndMode2D();
        EndDrawing();
    }

    UnloadSound(bgMusic);
    CloseAudioDevice();

    return 0;
}
