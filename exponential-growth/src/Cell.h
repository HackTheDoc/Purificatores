#pragma once
#include <SDL2/SDL.h>

typedef struct {
    SDL_Rect rect;
    bool occupied;
} Cell;

const SDL_Color freeCellColor       = {  0,   0,   0, 255};
const SDL_Color occupiedCellColor   = {191, 191, 191, 255};
