#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "Constants.h"
#include "Entity.h"

class Window {
    private:
        // Window vars
        SDL_Window* window;
        SDL_Event event;
        TTF_Font* font;
        SDL_Texture* pauseMessage;
        SDL_Rect pauseMessageRect;
        bool fullscreen;
        bool isPaused;

        std::vector<Entity*> entities;
        Species studiedSpecies;

    public:
        static SDL_Renderer* renderer;
        static SDL_Rect screen;
        static bool isRunning;

        Window();
        ~Window();
        void init(const char* title, int x, int y, int width, int height, bool fullscreen);
        void update();
        void render();
        void handleEvents();
        void kill();
};