enum buildingType {
    empty,
    shop,
    road,
    home  
};

class Building {
};

class Shop : Building {
};


class GameCell
{
    public:
        short posX;
        short posY;
        Building building;

        bool hasBuilding();
};
