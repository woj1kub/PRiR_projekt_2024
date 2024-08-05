#include <raylib-cpp.hpp>
#include <mutex>
using namespace std;
#ifndef GAME
#define GAME
#define CellSize 50
class Building
{ // To jest traktowane jako pusta
protected:
    Color _color = {255, 203, 0, 50};

public:
    // Rysowanie pustej planszy
    virtual void drawBuilding(short x, short y);
    virtual ~Building() = default;
};

class Shop : public Building
{
public:
    // Rysowanie sklepu planszy - można bez problemu zamienić na jaką teksture
    void drawBuilding(short x, short y) override;
};

class Home : public Building
{
public:
    // Rysowanie domu planszy - można bez problemu zamienić na jaką teksture
    void drawBuilding(short x, short y) override;
};

class Road : public Building
{
private:
    char roads;

public:
    Road(char roads) : roads(roads) {}
    // Rysowanie drogi planszy - można bez problemu zamienić na jaką teksture
    void drawBuilding(short x, short y) override;
};

class GameCell
{

private:
    short posX;
    short posY;
    Building *building;
    mutex lockBuilding;

public:
    GameCell(const GameCell &) = delete;            // No copying
    GameCell &operator=(const GameCell &) = delete; // No copying
    GameCell(GameCell &&other) noexcept;            // Move constructor
    GameCell &operator=(GameCell &&other) noexcept; // Move assignment operator
    ~GameCell();

    GameCell(short X, short Y);
    bool hasBuilding();
    bool isRoad()
    {
        lock_guard<mutex> lock(lockBuilding);
        return typeid(*building) == typeid(Road);
    }
    bool isRoadOrEmpty()
    {
        lock_guard<mutex> lock(lockBuilding);
        return typeid(*building) == typeid(Road) 
            || typeid(*building) == typeid(Building);
    }
    void drawCell();
    void setHome();
    void setShop();
    void setRoad(char roads);
    short getPosX() { return posX; }
    short getPosY() { return posY; }
};
#endif // GAME
