#include "../headers/game.h"

bool GameCell::hasBuilding()
{
    return typeid(*building) != typeid(Building);
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

GameCell::GameCell(short X, short Y)
    : posX(X), posY(Y), building(new Building()) {}

GameCell::GameCell(GameCell &&other) noexcept
    : posX(other.posX), posY(other.posY), building(other.building)
{
    other.building = nullptr;
}

GameCell &GameCell::operator=(GameCell &&other) noexcept
{
    if (this != &other)
    {
        std::unique_lock<std::mutex> lock1(lockBuilding, std::defer_lock);
        std::unique_lock<std::mutex> lock2(other.lockBuilding, std::defer_lock);
        std::lock(lock1, lock2);

        delete building;
        posX = other.posX;
        posY = other.posY;
        building = other.building;
        other.building = nullptr;
    }
    return *this;
}

GameCell::~GameCell()
{
    std::lock_guard<std::mutex> lock(lockBuilding);
    delete building;
}

void GameCell::drawCell()
{
    std::lock_guard<std::mutex> lock(lockBuilding);
    building->drawBuilding(posX, posY);
}

void GameCell::setHome()
{
    std::lock_guard<std::mutex> lock(lockBuilding);
    delete building;
    building = new Home();
}

void GameCell::setShop()
{
    std::lock_guard<std::mutex> lock(lockBuilding);
    delete building;
    building = new Shop();
}

void GameCell::setRoad(char roads)
{
    std::lock_guard<std::mutex> lock(lockBuilding);
    delete building;
    building = new Road(roads);
}
