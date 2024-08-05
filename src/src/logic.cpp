#include "../headers/logic.h"
void logic()
{
    double lastTimeUpdatedRoads = GetTime();          // Pobranie początkowego czasu
    const double intervalForUpdateRoads = 5.0 / 60.0; // Interwał co 10 klatek (60 FPS / 10 = 6 FPS)

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
        // To mozna przerobić na równoległe
        // Podzielić na wiersze lub kolumny ~ żeby było że zostało wykorzystane
        // Można to tak przerobić aby działo się tylko przy setach z GameCell
        double currentTime = GetTime();

        if (currentTime - lastTimeUpdatedRoads >= intervalForUpdateRoads)
        {
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
            lastTimeUpdatedRoads = currentTime;
        }
    }
}
