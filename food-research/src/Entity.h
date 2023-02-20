#pragma once
#include <SDL2/SDL.h>
#include "Food.h"

class Entity {
protected:
    int speed;
    int hungerRisingRate;

    int destinationX, destinationY;
    Food* targetedFood;

    SDL_Rect body;
    const SDL_Color* color;
    int hunger;

public:
    static int size;
    bool alive;

    Entity(int x, int y);
    ~Entity();
    void update();
    void draw();
    void moveTowardsDestination();
    void findNearestFoodSource();
    void eat();
};
