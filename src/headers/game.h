#include <raylib-cpp.hpp>
#include <mutex>
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
public:
    GameCell(const GameCell&) = delete; // No copying
    GameCell& operator=(const GameCell&) = delete; // No copying
    GameCell(GameCell&& other) noexcept; // Move constructor
    GameCell& operator=(GameCell&& other) noexcept; // Move assignment operator
    ~GameCell();

    GameCell(short X, short Y);
    bool hasBuilding();
    void drawCell();
    void setHome();
    void setShop();
    void setRoad(char roads);

private:
    short posX;
    short posY;
    Building *building;
    std::mutex lockBuilding;
};
#endif // GAME
