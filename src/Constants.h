#pragma once
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>

// Window relatives vars

const int scale = 4;

const int fontScale = scale;

const int defaultFontSize = 16;

// Study relatives vars

enum Species {
    NONE,
    SPECIES_1,
    NUMBER_OF_SPECIES
};

const std::map<Species, std::string> speciesName = {
    {NONE, "None"},
    {SPECIES_1, "Purificatore"}
};

const SDL_Color color[NUMBER_OF_SPECIES] = {
    {255, 255, 255, 255},
    {255, 204, 255, 255}
};
const SDL_Color textColor = {255, 0, 255, 255};
const SDL_Color borderColor = {0, 0, 0, 255};


const double birthRate = 0.4;

const double deathRate = 0.004;