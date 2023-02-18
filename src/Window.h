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
        bool fullscreen;

        SDL_Texture* pauseMessage;
        SDL_Rect pauseMessageRect;
        bool isPaused;

        int currentNumberOfEntities;
        SDL_TimerID nextDayTimer;

        SDL_Texture* numberOfLivingsText;
        SDL_Rect numberOfLivingsRect;

        void UpdateEntitiesCounter();

    public:
        Window();
        ~Window();
        void init(const char* title, int x, int y, int width, int height, bool fullscreen);
        void update();
        void render();
        void handleEvents();
        void kill();
        void NextDay();

        static void AddEntity();

        static SDL_Renderer* renderer;
        static SDL_Rect screen;
        static bool isRunning;
        static std::vector<Entity*> entities;
        static Species studiedSpecies;
};