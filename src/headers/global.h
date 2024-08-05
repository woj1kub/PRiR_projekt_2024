#include <atomic>
#include <mutex>
#include <vector>
#include "game.h"

using namespace std;
#ifndef GLOBAL
#define GLOBAL
extern atomic<bool> running; // To jest potrzebne aby po działaniu logic() można wyłączyć
extern mutex mapLock;              // Zabezpiecznie przed działaniem na mapie

extern size_t rows ;    // Ilość elemtów w wierszu
extern size_t columns ; // Ilość elementów w kolumnie

extern vector<GameCell> map;

// To jest offset potrzebny aby było mniejwięcej po środku
// Jak ktoś się chce pobawić bo nie wiem czy kamera jest 100% dobrze
extern float offSetTargetWidth ;
extern float offSetTargetHeight ;

#endif // GLOBAL