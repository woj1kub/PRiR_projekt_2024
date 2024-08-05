#include "../headers/cell_generation.h"
#include "../headers/randoms.h"
void generate_cell()
{

    double lastTimeUpdatedBuildings = GetTime();            // Pobranie początkowego czasu
    const double intervalForUpdateBuildings = 300.0 / 60.0; // Interwał co 300 klatek
    int houseNumberPerOneShop = 5;
    int countOfBuildings = 0;
    while (running.load())
    {
        double currentTime = GetTime();
        if (currentTime - lastTimeUpdatedBuildings >= intervalForUpdateBuildings)
        {
            lock_guard<mutex> guard(mapLock);
            // Tu można zrobić kopie mapy i robić na niej szukanie pozycji
            int positionOfNextBuilding;
            do
            {
                positionOfNextBuilding = generate_random_number(0, rows * columns);
            } while (!map[positionOfNextBuilding].isRoadOrEmpty());

            // Ale tutaj już pracować na globalnej mapie
            if (countOfBuildings % houseNumberPerOneShop == 0)
            {
                map[positionOfNextBuilding].setShop();
                countOfBuildings++;
            }
            else
            {
                map[positionOfNextBuilding].setHome();
                countOfBuildings++;
            }
            lastTimeUpdatedBuildings = GetTime();
        }
    }
}