#include "../headers/game.h"
#include "../headers/logic.h"
using namespace std;
#include "../headers/global.h"

bool GameCell::isRoad()
{
    lock_guard<mutex> lock(lockBuilding);
    return typeid(*building) == typeid(Road);
}
bool GameCell::isRoadOrEmpty()
{
    lock_guard<mutex> lock(lockBuilding);
    return typeid(*building) == typeid(Road) || typeid(*building) == typeid(Building);
}

bool GameCell::isShop()
{
    lock_guard<mutex> lock(lockBuilding);
    return typeid(*building) == typeid(Shop);
}

bool GameCell::isHome()
{
    lock_guard<mutex> lock(lockBuilding);
    return typeid(*building) == typeid(Home);
}

std::vector<GameCell *> GameCell::getNeighbors()
{
    std::vector<GameCell *> neighbors;
    int position = (posX * columns) + posY;

    // prawa (right)
    int neighborPos = position + columns;
    if (posX < rows - 1 && neighborPos >= 0 && neighborPos < rows * columns)
    {
        neighbors.push_back(&map[neighborPos]);
    }

    // lewa (left)
    neighborPos = position - columns;
    if (posX > 0 && neighborPos >= 0 && neighborPos < rows * columns)
    {
        neighbors.push_back(&map[neighborPos]);
    }

    // góra (up)
    neighborPos = position - 1;
    if (posY > 0 && neighborPos >= 0 && neighborPos < rows * columns)
    {
        neighbors.push_back(&map[neighborPos]);
    }

    // dół (down)
    neighborPos = position + 1;
    if (posY < columns - 1 && neighborPos >= 0 && neighborPos < rows * columns)
    {
        neighbors.push_back(&map[neighborPos]);
    }

    return neighbors;
}
int GameCell::findAllPathsToNearestShop()
{
    std::queue<std::pair<GameCell *, int>> queue;
    std::unordered_map<GameCell *, int> distances;
    std::unordered_map<GameCell *, int> paths;

    queue.push({this, 0});
    distances[this] = 0;
    paths[this] = 1;

    int minDistance = std::numeric_limits<int>::max();
    int pathCount = 0;

    while (!queue.empty())
    {
        auto [current, distance] = queue.front();
        queue.pop();

        if (current->isShop())
        {
            if (distance < minDistance)
            {
                minDistance = distance;
                pathCount = paths[current];
            }
            else if (distance == minDistance)
            {
                pathCount += paths[current];
            }
            continue;
        }

        for (GameCell *neighbor : current->getNeighbors())
        {
            if (neighbor->isRoadOrEmpty())
            {
                int newDistance = distance + 1;
                if (distances.find(neighbor) == distances.end())
                {
                    distances[neighbor] = newDistance;
                    paths[neighbor] = paths[current];
                    queue.push({neighbor, newDistance});
                }
                else if (newDistance == distances[neighbor])
                {
                    paths[neighbor] += paths[current];
                }
            }
        }
    }

    return pathCount;
}

bool GameCell::checkConnectionToStore()
{
    return findAllPathsToNearestShop() > 0;
}

bool GameCell::hasBuilding()
{

    lock_guard<mutex> lock(lockBuilding);
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

    float sizeRoad = 6.0f;
    // Rysowanie w zależności od bitów w zmiennej 'roads'
    if (roads == 0)
    {
        DrawCircle(centerX, centerY, sizeRoad * 1.3f, BLACK);
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
        unique_lock<mutex> lock1(lockBuilding, defer_lock);
        unique_lock<mutex> lock2(other.lockBuilding, defer_lock);
        lock(lock1, lock2);

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
    lock_guard<mutex> lock(lockBuilding);
    delete building;
}

void GameCell::drawCell()
{
    lock_guard<mutex> lock(lockBuilding);
    building->drawBuilding(posX, posY);
}

void GameCell::updateConnectionStatus()
{
    isConnectedToStore = findAllPathsToNearestShop() > 0;
}

void GameCell::setHome()
{
    unique_lock<mutex> lock(lockBuilding);
    delete building;
    building = new Home();
    lock.unlock();
    int pathsToShop = findAllPathsToNearestShop();
    leftRoadsTiles += pathsToShop;
    isConnectedToStore = pathsToShop > 0;
}

void GameCell::setShop()
{
    lock_guard<mutex> lock(lockBuilding);
    delete building;
    building = new Shop();
}

void GameCell::setRoad(char roads)
{
    lock_guard<mutex> lock(lockBuilding);
    delete building;
    building = new Road(roads);
    leftRoadsTiles -= 1;
}
