#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Unit.h"

class Window {
    private:
        const int scale = 4;
        const int caseSize = 16;

        // Window vars
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        bool fullscreen;
        int width, height;

        // Game vars
        enum units {
            WHITE,
            PURPLE,
            PINK,
            RED,
            GREEN,
            BLUE,
            NUMBER_OF_UNITS
        };
        SDL_Color color[NUMBER_OF_UNITS];
        
        Unit** units;

    public:
        static bool isRunning;

        Window();
        ~Window();
        void init(const char* title, int x, int y, int width, int height, bool fullscreen);
        void update();
        void render();
        void handleEvents();
        void kill();
};