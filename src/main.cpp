#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Window.h"

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   640

Window window;

int main() {
    const int FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;
    
    window.init(
        "PURIFICATORES",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        false
    );
    while(Window::isRunning) {
        frameStart = SDL_GetTicks();

        window.handleEvents();
        window.update();
        window.render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    window.kill();

    return 0;
}
