#include <raylib-cpp.hpp>
#include <mutex>

#define CellSize 50
class Building
{ // To jest traktowane jako pusta
protected:
    Color _color = {255, 203, 0, 50};

public:
    // Rysowanie pustej planszy
    virtual void drawBuilding(short x, short y);
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
    short posX;
    short posY;
    Building *building;
    std::mutex lockBuilding;
    bool hasBuilding();
    GameCell(short X, short Y)
    {

        posX = X;
        posY = Y;
        building = new Building();
    }
    void drawCell()
    {
        std::unique_lock<std::mutex> lock(lockBuilding);
        building->drawBuilding(posX, posY);
        lock.unlock();
    }
    void setHome()
    {
        std::unique_lock<std::mutex> lock(lockBuilding);
        delete building;
        building = new Home();
        lock.unlock();
    }
    void setShop()
    {
        std::unique_lock<std::mutex> lock(lockBuilding);
        delete building;
        building = new Shop();
        lock.unlock();
    }
    void setRoad(char roads)
    {
        std::unique_lock<std::mutex> lock(lockBuilding);
        delete building;
        building = new Road(roads);
        lock.unlock();
    }
};
