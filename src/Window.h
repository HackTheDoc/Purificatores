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

        std::vector<Entity*> entities;
        Species studiedSpecies;
        int currentNumberOfEntities;
        SDL_Texture* studyData;
        SDL_Rect studyDataRect;
        SDL_TimerID nextDayTimer;
    
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
        void NextDay();
};