#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "Constants.h"
#include "Entity.h"
#include "Food.h"

class Simulation {
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
    SDL_Texture* numberOfEntitiesText;
    SDL_Rect numberOfEntitiesRect;

    int currentNumberOfFoodSources;
    SDL_Texture* numberOfFoodSourcesText;
    SDL_Rect numberOfFoodSourcesRect;

    int day;
    SDL_Texture* dayCounterText;
    SDL_Rect dayCounterRect;
    SDL_TimerID nextDayTimer;

    void UpdateEntitiesCounter();
    void UpdateFoodSourcesCounter();
    void UpdateCounters();

    void ClearEntities();
    void ClearFoodSources();

public:
    Simulation();
    ~Simulation();
    void initWindow(const char* title, int x, int y, int width, int height, bool fullscreen);
    void start();
    void update();
    void render();
    void handleEvents();
    void kill();
    void NextDay();

    static void AddEntity();
    static void AddFoodSource();

    static SDL_Renderer* renderer;
    static SDL_Rect screen;
    static bool isRunning;
    static std::vector<Entity*> entities;
    static std::vector<Food*> foodSources;
};