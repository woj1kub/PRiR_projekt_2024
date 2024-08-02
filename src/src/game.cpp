#include "../headers/game.h"

bool GameCell::hasBuilding()
{
    return 0;
}

void Building::drawBuilding(short x, short y)
{
    DrawRectangleLines(x * CellSize, y * CellSize, CellSize, CellSize, BLACK);
}

void Shop::drawBuilding(short x, short y)
{
    Building::drawBuilding(x, y);
    DrawRectangleLines((x * CellSize) + 5, (y * CellSize) + 5, CellSize - 10, CellSize - 10, BLACK);
    DrawRectangle((x * CellSize) + 5, (y * CellSize) + 5, CellSize - 10, CellSize - 10, _color);
    int centerX = (x * CellSize) + 5 + (CellSize - 10) / 2;
    int centerY = (y * CellSize) + 5 + (CellSize - 10) / 2;

    DrawCircleLines(centerX, centerY, (CellSize / 2) - 10, BLACK);
}

void Home::drawBuilding(short x, short y)
{
    Building::drawBuilding(x, y);
    DrawRectangleLines((x * CellSize) + 5, (y * CellSize) + 5, CellSize - 10, CellSize - 10, BLACK);
    DrawRectangle((x * CellSize) + 5, (y * CellSize) + 5, CellSize - 10, CellSize - 10, _color);
    int centerX = (x * CellSize) + 5 + (CellSize - 10) / 2;

    DrawLine(centerX, (y * CellSize) + 5, centerX, (y * CellSize) + 5 + CellSize - 10, BLACK);
}

void Road::drawBuilding(short x, short y)
{

    Building::drawBuilding(x, y);
    int centerX = (x * CellSize) + 5 + (CellSize - 10) / 2;
    int centerY = (y * CellSize) + 5 + (CellSize - 10) / 2;

    float sizeRoad = 4.0f;
    // Rysowanie w zależności od bitów w zmiennej 'roads'
    if (roads == 0)
    {
        DrawCircle(centerX, centerY, sizeRoad * 1.5f, BLACK);
    }

    if (roads & 0x01) // pierwszy bit
    {
        DrawLineEx({(float)centerX, (float)centerY}, {(float)centerX, (float)((y * CellSize))}, sizeRoad, BLACK); // do góry
    }

    if (roads & 0x02) // drugi bit
    {
        DrawLineEx({(float)centerX, (float)centerY}, {(float)centerX, (float)((y * CellSize) + CellSize)}, sizeRoad, BLACK); // w dół
    }

    if (roads & 0x04) // trzeci bit
    {
        DrawLineEx({(float)centerX, (float)centerY}, {(float)((x * CellSize)), (float)centerY}, sizeRoad, BLACK); // w lewo
    }

    if (roads & 0x08) // czwarty bit
    {
        DrawLineEx({(float)centerX, (float)centerY}, {(float)((x * CellSize) + CellSize), (float)centerY}, sizeRoad, BLACK); // w prawo
    }
}
