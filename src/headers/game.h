#include <raylib-cpp.hpp>

#define CellSize 50
class Building { // To jest traktowane jako pusta 
    protected:
        Color _color = { 255, 203, 0, 50 };  
    public:
        void drawBuilding(short x , short y){
            DrawRectangleLines(x * CellSize , y * CellSize , CellSize , CellSize , BLACK);
        }
};

class Shop : Building {
    public:
        void drawBuilding(short x, short y){
            Building::drawBuilding(x,y);
            DrawRectangleLines((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , BLACK);
            DrawRectangle((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , _color);
            int centerX = (x*CellSize) + 5 + (CellSize - 10) / 2;
            int centerY = (y*CellSize) + 5 + (CellSize - 10) / 2;

            DrawCircleLines(centerX, centerY,(CellSize/2)-10 , BLACK);
        }
};

class Home : Building {
    public:
        void drawBuilding(short x, short y){
            Building::drawBuilding(x,y);
            DrawRectangleLines((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , BLACK);
            DrawRectangle((x * CellSize) + 5 , (y * CellSize) + 5 , CellSize-10 , CellSize-10 , _color);
            int centerX = (x*CellSize) + 5 + (CellSize - 10) / 2;

            DrawLine(centerX, (y * CellSize) + 5, centerX, (y * CellSize) + 5 + CellSize - 10, BLACK);
        }
};

class Road : Building {
    private:
        
};

class GameCell
{
    public:
        short posX;
        short posY;
        Building building;

        bool hasBuilding();
};
