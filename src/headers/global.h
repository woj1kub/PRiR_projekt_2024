#include <atomic>
#include <mutex>
#include <vector>
#include "game.h"
#include <random>

using namespace std;
#ifndef GLOBAL
#define GLOBAL
extern atomic<bool> running; // To jest potrzebne aby po działaniu logic() można wyłączyć
extern atomic<bool> loseState; // To jest potrzebne aby po działaniu logic() można wyłączyć
extern mutex mapLock;        // Zabezpiecznie przed działaniem na mapie
extern mutex timeMutex;      // Zabezpiecznie przed działaniem na mapie

extern size_t rows;    // Ilość elemtów w wierszu
extern size_t columns; // Ilość elementów w kolumnie

extern vector<GameCell> map;

// To jest offset potrzebny aby było mniejwięcej po środku
// Jak ktoś się chce pobawić bo nie wiem czy kamera jest 100% dobrze
extern float offSetTargetWidth;
extern float offSetTargetHeight;

extern int timeInt;
extern unsigned int seed;
extern atomic<int> Points;
extern atomic<int> leftRoadsTiles;
extern mt19937 generator;

#endif // GLOBAL