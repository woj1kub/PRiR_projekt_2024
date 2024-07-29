#include <raylib-cpp.hpp>

#define CellSize 50
class Building { // To jest traktowane jako pusta 
    protected:
        Color _color = { 255, 203, 0, 50 };  
    public:
        virtual void drawBuilding(short x , short y){
            DrawRectangleLines(x * CellSize , y * CellSize , CellSize , CellSize , BLACK);
        }
};

class Shop : public Building {
    public:
        void drawBuilding(short x, short y) override {
            Building::drawBuilding(x,y);
            DrawRectangleLines((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , BLACK);
            DrawRectangle((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , _color);
            int centerX = (x*CellSize) + 5 + (CellSize - 10) / 2;
            int centerY = (y*CellSize) + 5 + (CellSize - 10) / 2;

            DrawCircleLines(centerX, centerY,(CellSize/2)-10 , BLACK);
        }
};

class Home : public Building {
    public:
        void drawBuilding(short x, short y) override {
            Building::drawBuilding(x,y);
            DrawRectangleLines((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , BLACK);
            DrawRectangle((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , _color);
            int centerX = (x*CellSize) + 5 + (CellSize - 10) / 2;

            DrawLine(centerX, (y * CellSize) + 5, centerX, (y * CellSize) + 5 + CellSize - 10, BLACK);
        }
};

class Road : public Building {
private:
    char roads;
public:
    Road(char roads) : roads(roads) {}

    void drawBuilding(short x, short y) override {
        Building::drawBuilding(x, y);
        int centerX = (x * CellSize) + 5 + (CellSize - 10) / 2;
        int centerY = (y * CellSize) + 5 + (CellSize - 10) / 2;

        // Rysowanie w zależności od bitów w zmiennej 'roads'
        if (roads & 0x01) { // pierwszy bit
            DrawLineEx({(float)centerX, (float)centerY}, {(float)centerX, (float)((y * CellSize))}, 3.0f, BLACK); // do góry
        }
        if (roads & 0x02) { // drugi bit
            DrawLineEx({(float)centerX, (float)centerY}, {(float)centerX, (float)((y * CellSize) + CellSize )}, 3.0f, BLACK); // w dół
        }
        if (roads & 0x04) { // trzeci bit
            DrawLineEx({(float)centerX, (float)centerY}, {(float)((x * CellSize)), (float)centerY}, 3.0f, BLACK); // w lewo
        }
        if (roads & 0x08) { // czwarty bit
            DrawLineEx({(float)centerX, (float)centerY}, {(float)((x * CellSize) + CellSize), (float)centerY}, 3.0f, BLACK); // w prawo
        }
    }
};

class GameCell
{
    public:
        short posX;
        short posY;
        Building* building;

        bool hasBuilding();
        GameCell(short X, short Y){
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
