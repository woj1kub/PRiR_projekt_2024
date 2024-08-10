#include <raylib-cpp.hpp>
#include <vector>
#include "headers/game.h"
#include "headers/global.h"
#include "headers/logic.h"
#include "headers/cell_generation.h"
#include "headers/backup.h"
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;
// Zmienne globalne ~ nazwy tymczasowe
// Zmienna globalne znajdują się w global.h
// Logic znajduje się w logic.h

void DrawMenu(raylib::Window &w)
{
    bool inMenu = true;
    while (inMenu && !w.ShouldClose())
    {
        // Rysowanie menu
        BeginDrawing();
        ClearBackground(RAYWHITE);

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Przyciski
        int buttonWidth = 300;
        int buttonHeight = 60;
        int buttonX = (screenWidth - buttonWidth) / 2;
        int buttonPlayY = (screenHeight - buttonHeight) / 2 - 50;
        int buttonExitY = buttonPlayY + 100;

        DrawRectangle(buttonX, buttonPlayY, buttonWidth, buttonHeight, DARKGREEN);
        DrawRectangleLines(buttonX, buttonPlayY, buttonWidth, buttonHeight, BLACK);
        DrawText("Play", buttonX + 100, buttonPlayY + 15, 30, WHITE);

        DrawRectangle(buttonX, buttonExitY, buttonWidth, buttonHeight, MAROON);
        DrawRectangleLines(buttonX, buttonExitY, buttonWidth, buttonHeight, BLACK);
        DrawText("Exit", buttonX + 100, buttonExitY + 15, 30, WHITE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();

            if (mousePosition.x > buttonX && mousePosition.x < buttonX + buttonWidth)
            {
                if (mousePosition.y > buttonPlayY && mousePosition.y < buttonPlayY + buttonHeight)
                {
                    inMenu = false; // Przejście do gry
                }
                else if (mousePosition.y > buttonExitY && mousePosition.y < buttonExitY + buttonHeight)
                {
                    w.Close(); // Zakończenie gry
                    break;
                }
            }
        }

        EndDrawing();
    }
}

int main()
{
    // Initialization
    int screenWidth = 1600;
    int screenHeight = 900;
    raylib::Window w(screenWidth, screenHeight, "Road Maker Speedrun");
    InitAudioDevice();

    DrawMenu(w);

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
        {
            lock_guard<mutex> guard(mapLock);
            for (auto &&i : map)
            {
                i.drawCell();
            }
        }
        {
            lock_guard<mutex> guardTime(timeMutex);
            string timeString = to_string(timeInt);
            DrawText(timeString.c_str(), 1050, 0, 50.0f, BLACK);
        }

        string seedString = to_string(seed);
        DrawText("Seed", 1050, 60, 30.0f, BLACK);
        DrawText(seedString.c_str(), 1050, 90, 30.0f, BLACK);

        string leftRoadsTilesString = to_string(leftRoadsTiles);
        DrawText("Roads Tiles", 1050, 120, 30.0f, BLACK);
        DrawText(leftRoadsTilesString.c_str(), 1050, 150, 30.0f, BLACK);

        if (loseState.load() == true)
        {
            // Tu dodać rysowanie okna powiadomienia o zakończenia gry oraz przegranej punkty
            // Trzeba dodac rysowanie guzików oraz ich wykrycie w "logic".
            int positionX = 350, positionY = 200, witdh = 300, height = 200;
            int buttonH = 45;
            int buttonW = 100;
            int greenButtonX = positionX + 15, greenButtonY = positionY + height - 60;
            int redButtonX = positionX + witdh - 115, redButtonY = positionY + height - 60;
            int buttonTextSize = 25;
            string pointsString = "Zdobyte punkty: " + to_string(points.load());

            DrawRectangle(positionX, positionY, witdh, height, GRAY);
            DrawRectangleLines(positionX, positionY, witdh, height, BLACK);
            DrawText("Przegrales :(", positionX + 15, positionY + 45, 40, RED);
            DrawText(pointsString.c_str(), positionX + 15, positionY + 90, 25, BLACK);

            DrawRectangle(greenButtonX, greenButtonY, buttonW, buttonH, DARKGREEN);
            DrawRectangleLines(greenButtonX, greenButtonY, buttonW, buttonH, BLACK);
            DrawText("Znowu", greenButtonX + 12, greenButtonY + 10, buttonTextSize, WHITE);

            DrawRectangle(redButtonX, redButtonY, buttonW, buttonH, MAROON);
            DrawRectangleLines(redButtonX, redButtonY, buttonW, buttonH, BLACK);
            DrawText("Wyjdz", redButtonX + 15, redButtonY + 10, buttonTextSize, WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                Vector2 mousePosition = GetMousePosition();
                mousePosition.x += offSetTargetWidth;
                mousePosition.y += offSetTargetHeight;
                if (mousePosition.x > greenButtonX && mousePosition.x < greenButtonX + buttonW && mousePosition.y > greenButtonY && mousePosition.y < greenButtonY + buttonH)
                {
                    timeInt = 50; // Taki sam jaki ustawimy na stracie. Można do tego inaczej zrobić że jest ustawiana w menu startowym
                    lock_guard<mutex> lock(mapLock);
                    backup();
                    for (auto &&i : map)
                    {
                        i.setEmpty();
                    }
                    points.store(0);
                    leftRoadsTiles.store(0);
                    seed = time(0);
                    generator = mt19937(static_cast<unsigned int>(seed));
                    running.store(false);

                    logicThread.join();
                    generateCellThread.join();
                    running.store(true);
                    logicThread = thread(logic);
                    generateCellThread = thread(generate_cell);
                    loseState.store(false);
                }
                if (mousePosition.x > redButtonX && mousePosition.x < redButtonX + buttonW && mousePosition.y > redButtonY && mousePosition.y < redButtonY + buttonH)
                {
                    // Funkcja przy robieniu czegoś z czerwonym guzikiem
                    w.Close();
                    break;
                }
            }
        }

        EndMode2D();
        EndDrawing();
    }
    // Zamykanie programu - czynności związane z zamykaniem programu
    running.store(false);
    logicThread.join();
    generateCellThread.join();
    backup();
    UnloadSound(bgMusic);
    CloseAudioDevice();

    return 0;
}
