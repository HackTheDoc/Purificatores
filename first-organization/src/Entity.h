#pragma once
#include <SDL2/SDL.h>

class Entity {
protected:
    int speed;
    int destinationX, destinationY;

    SDL_Rect body;
    const SDL_Color* color;

public:
    static int size;

    Entity();
    Entity(int x, int y);
    ~Entity();
    virtual void update() {}
    void draw();
    void moveTowardsDestination();
};
