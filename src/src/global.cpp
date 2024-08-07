#include "../headers/global.h"

atomic<bool> running(true);    // To jest potrzebne aby po działaniu logic() można wyłączyć
atomic<bool> loseState(false); // To jest potrzebne aby po działaniu logic() można wyłączyć
mutex timeMutex;               // Zabezpiecznie przed działaniem na mapie
mutex mapLock;                 // Zabezpiecznie przed działaniem na mapie

size_t rows = 20;    // Ilość elemtów w wierszu
size_t columns = 15; // Ilość elementów w kolumnie

vector<GameCell> map;

// To jest offset potrzebny aby było mniejwięcej po środku
// Jak ktoś się chce pobawić bo nie wiem czy kamera jest 100% dobrze
float offSetTargetWidth = -300.0f;
float offSetTargetHeight = -50.0f;

int timeInt = 10;

atomic<int> leftRoadsTiles(0);

unsigned int seed = time(0);
atomic<int> Points(0);
mt19937 generator(static_cast<unsigned int>(seed));