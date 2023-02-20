#pragma once
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>

// Window relatives vars

const int scale = 2;

const int fontScale = scale * 2;

const int defaultFontSize = 16;

// Study relatives vars

const SDL_Color entityColor = {255, 255, 255, 255};
const SDL_Color masterColor = {255, 204, 255, 255};
const SDL_Color slaveColor  = {255, 102, 255, 255};
const SDL_Color freeColor   = {128, 255, 128, 255};
const SDL_Color borderColor = {  0,   0,   0, 255};
const SDL_Color textColor   = {  0,   0,   0, 255};

const int startingSlavePopulation = 1;
const int startingFreePopulation = 0;

const int entityDefaultSize = 32;

const int dayLength = 1000 * 5;
