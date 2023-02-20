#pragma once
#include <SDL2/SDL.h>

class Entity {
    private:
    static const int speed = 4;

    bool alive;
    int species;
    int destinationX, destinationY;


    public:
    static int size;
    SDL_Rect body;
    int reproducing;

    Entity();
    Entity(int x, int y, int species);
    ~Entity();
    void update();
    void draw();
    void reproduce();
};