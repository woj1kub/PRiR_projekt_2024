#include "../headers/logic.h"

void logic()
{
    double lastTimeUpdatedRoads = GetTime();          // Pobranie początkowego czasu
    double lastTimeUpdatedTime = GetTime();           // Pobranie początkowego czasu
    const double intervalForUpdateRoads = 5.0 / 60.0; // Interwał co 5 klatek
    const double intervalForUpdateTime = 1.0;         // Interwał co 60 klatek

    while (running.load())
    {
        if (loseState==true)
        {
            continue;
        }
        
        // Wstawianie dróg
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            lock_guard<mutex> guard(mapLock);
            Vector2 mousePosition = GetMousePosition();
            int pos = ((int)(mousePosition.y + offSetTargetHeight) / CellSize) + (columns * ((int)(mousePosition.x + offSetTargetWidth) / CellSize));

            int max = rows * columns;
            if (pos < columns * rows && pos >= 0)
            {
                if (!map[pos].hasBuilding() && leftRoadsTiles > 0)
                {
                    map[pos].setRoad(0);
                    leftRoadsTiles.fetch_sub(1);
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            lock_guard<mutex> guard(mapLock);
            Vector2 mousePosition = GetMousePosition();
            int pos = ((int)(mousePosition.y + offSetTargetHeight) / CellSize) + (columns * ((int)(mousePosition.x + offSetTargetWidth) / CellSize));

            int max = rows * columns;
            if (pos < columns * rows && pos >= 0)
            {
                if (map[pos].isRoad())
                {
                    map[pos].setEmpty();
                    leftRoadsTiles.fetch_add(1);
                }
            }
        }
        // Auktulizacja dróg
        // To mozna przerobić na równoległe
        // Podzielić na wiersze lub kolumny ~ żeby było że zostało wykorzystane
        // Można to tak przerobić aby działo się tylko przy setach z GameCell
        // od razu sprawdzamy czy jest połączone w tym mniejscu bo i tak lecimy po mapie

        double currentTime = GetTime();
        bool notConnected = false;

        if (currentTime - lastTimeUpdatedRoads >= intervalForUpdateRoads)
        {
            lock_guard<mutex> guard(mapLock);
            for (int i = 0; i < rows * columns; i++)
            {
                if (map[i].isHome())
                {
                    map[i].updateConnectionStatus();
                }

                if (!notConnected)
                {
                    if (!map[i].getIsConnectedToStore())
                    {
                        notConnected = true;
                    }
                }

                if (map[i].isRoad())
                {
                    char road = 0b0;
                    int position = (map[i].getPosY()) + ((map[i].getPosX()) * columns);

                    // prawa
                    int neighborPos = position + columns;
                    if (neighborPos >= 0 && neighborPos < rows * columns && map[i].getPosX() < rows - 1)
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

        // Dodawanie czasu na ten moment (1s). Trzeba inaczej to zrobić
        if (currentTime - lastTimeUpdatedTime >= intervalForUpdateTime && notConnected)
        {

            lock_guard<mutex> guardTime(timeMutex);
            timeInt--;
            lastTimeUpdatedTime = currentTime;
        }
        
        if (timeInt == 0)
        {
            loseState.store(true);
        }
        
 
    }
}
