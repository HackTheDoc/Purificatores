#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "Constants.h"
#include "Entity.h"
#include "Master.h"
#include "Slave.h"

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

    int currentNumberOfSlaves;
    SDL_Texture* numberOfSlavesText;
    SDL_Rect numberOfSlavesRect;

    int currentNumberOfFree;
    SDL_Texture* numberOfFreeText;
    SDL_Rect numberOfFreeRect;

    int day;
    SDL_Texture* dayCounterText;
    SDL_Rect dayCounterRect;
    SDL_TimerID nextDayTimer;

    void UpdateSlavesCounter();
    void UpdateFreesCounter();
    void UpdateCounters();
    void ClearEntities();

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
    static void AddMaster();

    static SDL_Renderer* renderer;
    static SDL_Rect screen;
    static bool isRunning;
    static Master* master;
    static std::vector<Slave*> entities;
};