#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Window.h"

#define SCREEN_WIDTH    2560
#define SCREEN_HEIGHT   1440

Window window;

int main() {
    const float FPS = 60;
    const int frameDelay = 1000/FPS;

    Uint32 frameStart;
    int frameTime;
    
    window.init(
        "PURIFICATORES",
        0, 0,                           // place window at top left corner of the screen
        SCREEN_WIDTH, SCREEN_HEIGHT,
        false
    );
/*
    SDL_TimerID updateTimer = SDL_AddTimer(500, [](Uint32 interval, void* param) {
        Window* window = static_cast<Window*>(param);
        window->update();
        return interval;
    }, &window);*/

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

    //SDL_RemoveTimer(updateTimer);

    window.kill();

    return 0;
}
