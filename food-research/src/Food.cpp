#include "Food.h"
#include "Simulation.h"
#include "Constants.h"

int Food::size = foodDefaultSize * scale;

Food::Food(int x, int y) {
    this->x = x;
    this->y = y;
    this->body = {x, y, size, size};
    durability = 1000 * 60; // food source expire after 1 day (1 minute) 
    expired = false;
}

Food::~Food() {}

void Food::update() {
    durability--;
    if (durability <= 0) {
        expired = true;
    }
}

void Food::draw() {
    // content
    SDL_SetRenderDrawColor(Simulation::renderer, foodColor.r, foodColor.g, foodColor.b, foodColor.a);
    SDL_RenderFillRect(Simulation::renderer, &body);
    // border
    SDL_SetRenderDrawColor(Simulation::renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(Simulation::renderer, &body);
}
