#include <raylib-cpp.hpp>
#include <vector>
#include "headers/game.h"
#include "headers/global.h"
#include "headers/logic.h"
#include "headers/cell_generation.h"
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;
// Zmienne globalne ~ nazwy tymczasowe
// Zmienna globalne znajdują się w global.h
// Logic znajduje się w logic.h

int main()
{
    // Initialization

    int screenWidth = 1600;
    int screenHeight = 900;
    raylib::Color textColor(LIGHTGRAY);
    raylib::Window w(screenWidth, screenHeight, "Raylib C++ Starter Kit Example");

    InitAudioDevice();

    Sound bgMusic = LoadSound("assets/audio/chipi.wav");
    PlaySound(bgMusic);

    Camera2D camera = {0};
    camera.target = {screenWidth / 2.0f + offSetTargetWidth, screenHeight / 2.0f + offSetTargetHeight};
    camera.offset = {screenWidth / 2.0f, screenHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    // Inicjalizacja tabeli/mapy
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            map.push_back(GameCell(i, j));
        }
    }

    // Przykładowe użycie
    // Można zrobić tutaj początkowy stan mapy
    map[10].setHome();
    map[11].setShop();
    map[12].setRoad(0b0001);
    map[13].setRoad(0b0011);
    map[14].setRoad(0b0111);
    map[15].setRoad(0b1111);
    map[16].setRoad(0b0);

    SetTargetFPS(60);
    thread logicThread(logic);
    thread generateCellThread(generate_cell);
    // Main game loop

    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        // Rysowanie całej mapy
        {
            lock_guard<mutex> guard(mapLock);

            for (auto &&i : map) // To można zostawić
            {
                i.drawCell();
            }
        }
        {
            lock_guard<mutex> guardTime(timeMutex);
            string timeString = to_string(timeInt);
            DrawText(timeString.c_str(), -60, 0, 50.0f, BLACK);
        }
        EndMode2D();
        EndDrawing();
    }
    // Zamykanie programu - czynności związane z zamykaniem programu
    running.store(false);
    logicThread.join();
    generateCellThread.join();
    UnloadSound(bgMusic);
    CloseAudioDevice();

    return 0;
}
