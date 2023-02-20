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
const SDL_Color walkerColor = {255, 204, 255, 255};
const SDL_Color runnerColor = {255, 102, 255, 255};
const SDL_Color foodColor   = {128, 255, 128, 255};
const SDL_Color borderColor = {  0,   0,   0, 255};
const SDL_Color textColor   = {  0,   0,   0, 255};

const int startingPopulation = 50;

const int startingFoodSources = 50;
const double foodSourcesRarificationRate = 0.8;

const int entityDefaultSize = 32;
const int foodDefaultSize = 8;
