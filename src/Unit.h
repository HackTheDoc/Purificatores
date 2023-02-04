#pragma once
#include <SDL2/SDL.h>

typedef struct {
    int id = 0;
    SDL_Rect rect = {0, 0, 16, 16};
    int surroundedBy = 0;
    bool beenConqueredThisTurn = false;
} Unit;