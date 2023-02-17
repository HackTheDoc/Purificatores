#pragma once
#include <SDL2/SDL.h>

class Entity {
    private:
    static const int speed = 10;

    SDL_Rect body;
    bool alive;
    int species;

    public:
    static int size;

    Entity();
    Entity(int x, int y, int species);
    ~Entity();
    void update();
    void draw();
};