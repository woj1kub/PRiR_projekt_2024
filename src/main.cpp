#include <raylib-cpp.hpp>
#include <vector>
#include "headers/game.h"
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;
// Zmienne globalne ~ nazwy tymczasowe
atomic<bool> running(true);
mutex mapLock;

size_t rows = 20;
size_t columns = 15;

vector<GameCell> map;

float offSetTargetWidth = -300;
float offSetTargetHeight = -50;

int screenWidth = 1600;
int screenHeight = 900;

void logic()
{
    while (running.load())
    {
        lock_guard<mutex> guard(mapLock);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();
            int pos = ((int)(mousePosition.y + offSetTargetHeight) / CellSize) + (columns * ((int)(mousePosition.x + offSetTargetWidth) / CellSize));
            char roads = 0b0;
            if (!map[pos].hasBuilding())
            {
                map[pos].setRoad(0);
            }
        }

        // Auktulizacja dróg
        for (int i = 0; i < rows * columns; i++)
        {
            if (map[i].isRoad())
            {
                char road = 0b0;
                int position = (map[i].getPosY()) + ((map[i].getPosX()) * columns);

                // prawa
                int neighborPos = position + columns;
                if (neighborPos >= 0 && neighborPos < rows * columns && map[i].getPosX() < rows)
                {
                    road += map[neighborPos].hasBuilding() ? 0b1000 : 0;
                }

                // Lewa
                neighborPos = position - columns;
                if (neighborPos >= 0 && neighborPos < rows * columns && map[i].getPosX() > 0)
                {
                    road += map[neighborPos].hasBuilding() ? 0b0100 : 0;
                }

                // Góra
                neighborPos = position - 1;
                if (neighborPos >= 0 && neighborPos < rows * columns && map[i].getPosY() > 0)
                {
                    road += map[neighborPos].hasBuilding() ? 0b0001 : 0;
                }

                // Dół
                neighborPos = position + 1;
                if (neighborPos >= 0 && neighborPos < rows * columns && map[i].getPosY() < columns - 1)
                {
                    road += map[neighborPos].hasBuilding() ? 0b0010 : 0;
                }

                map[i].setRoad(road);
            }
        }
    }
}

int main()
{
    // Initialization

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

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < columns; j++)
        {
            map.push_back(GameCell(i, j));
        }
    }

    map[10].setHome();
    map[11].setShop();
    map[12].setRoad(0b0001);
    map[13].setRoad(0b0011);
    map[14].setRoad(0b0111);
    map[15].setRoad(0b1111);
    map[16].setRoad(0b0);

    SetTargetFPS(60);
    thread logicThread(logic);
    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        {
            lock_guard<mutex> guard(mapLock);

            for (auto &&i : map) // To można zostawić
            {
                i.drawCell();
            }
        }
        EndMode2D();
        EndDrawing();
    }
    running.store(false);
    logicThread.join();
    UnloadSound(bgMusic);
    CloseAudioDevice();

    return 0;
}
