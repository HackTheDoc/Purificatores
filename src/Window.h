#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Window {
    private:
        const int scale = 4;
        const int fontScale = scale * 2;
        const int defaultFontSize = 16;

        // Window vars
        SDL_Window* window;
        SDL_Event event;
        TTF_Font* font;
        SDL_Texture* pauseMessage;
        SDL_Rect pauseMessageRect;
        bool fullscreen;
        bool isPaused;

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