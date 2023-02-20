#pragma once
#include <SDL2/SDL.h>

class Food {
private:
    SDL_Rect body;
    int durability;

public:
    int x, y;
    bool expired;

    Food(int x, int y);
    ~Food();
    void update();
    void draw();

    static int size;
};

