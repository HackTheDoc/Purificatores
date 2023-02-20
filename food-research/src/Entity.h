#pragma once
#include <SDL2/SDL.h>
#include "Food.h"

class Entity {
private:
    static const int speed = 4;

    int destinationX, destinationY;
    Food* targetedFood;

    SDL_Rect body;
    int hunger;

public:
    static int size;
    bool alive;

    Entity(int x, int y);
    ~Entity();
    void update();
    void draw();
    void findNearestFoodSource();
    void eat();
    void starve();
};
