#include <raylib-cpp.hpp>

#define CellSize 50
class Building { // To jest traktowane jako pusta 
    protected:
        Color _color = { 255, 203, 0, 50 };  
    public:
        virtual void drawBuilding(short x , short y);
};

class Shop : public Building {
    public:
        void drawBuilding(short x, short y) override ;
};

class Home : public Building {
    public:
        void drawBuilding(short x, short y) override ;
};

class Road : public Building {
private:
    char roads;
public:
    Road(char roads) : roads(roads) {}

    void drawBuilding(short x, short y) override ;
};

class GameCell
{
    public:
        short posX;
        short posY;
        Building* building;

        bool hasBuilding();
        GameCell(short X, short Y) {
            posX = X;
            posY=Y;
            building = new Building();
        }
        void drawCell(){
            building->drawBuilding(posX,posY);
        }
        void setHome(){
            delete building;
            building = new Home();
        }
        void setShop(){
            delete building;
            building = new Shop();
        }
        void setRoad(char roads){
            delete building;
            building = new Road(roads);
        }
};
