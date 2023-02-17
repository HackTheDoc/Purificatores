#pragma once
#include <iostream>
#include <map>
#include <string>
#include <SDL2/SDL.h>

const int scale = 4;

const int fontScale = scale;

const int defaultFontSize = 16;



enum Species {
    NONE,
    SPECIES_1,
    NUMBER_OF_SPECIES
};

const std::map<Species, std::string> speciesName = {
    {NONE, "None"},
    {SPECIES_1, "Species_1"}
};

const SDL_Color color[NUMBER_OF_SPECIES] = {
    {255, 255, 255, 255},
    {255, 204, 255, 255}
};
const SDL_Color textColor = {255, 0, 255, 255};
const SDL_Color borderColor = {0, 0, 0, 255};


const double birthChance = 0.6;

const double deathChance = 0.3;