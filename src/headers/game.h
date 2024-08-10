#include <raylib-cpp.hpp>
#include <mutex>
#include <queue>
#include <set>
#include <unordered_map>
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
    char getRoad();
};

class GameCell
{

protected:
    short posX;
    short posY;
    Building *building;
    mutex lockBuilding;
    bool isConnectedToStore = true; // To będzie potrzebne do sprawdzania czy dom jest podłączony do sklepu
public:
    // To jest zrobione aby działało :)
    GameCell(const GameCell &) = delete;            // No copying
    GameCell &operator=(const GameCell &) = delete; // No copying
    GameCell(GameCell &&other) noexcept;            // Move constructor
    GameCell &operator=(GameCell &&other) noexcept; // Move assignment operator
    ~GameCell();

    GameCell(short X, short Y);
    bool hasBuilding();
    bool isRoad();
    bool isRoadOrEmpty();
    bool isShop();
    bool isHome();
    vector<GameCell *> getNeighbors();

    int findAllPathsToNearestShop();

    bool checkConnectionToStore();

    bool getIsConnectedToStore() { return isConnectedToStore; }
    void drawCell();
    void updateConnectionStatus();
    void setHome();
    void setShop();
    void setRoad(char roads);
    void setEmpty();
    short getPosX() { return posX; }
    short getPosY() { return posY; }

    string toString();
};
#endif // GAME
