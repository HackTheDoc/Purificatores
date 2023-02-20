#include "Simulation.h"
#include <cstdlib>
#include <ctime>


SDL_Renderer* Simulation::renderer = nullptr;
SDL_Rect Simulation::screen = {0, 0, 0, 0};
bool Simulation::isRunning = false;
Master* Simulation::master = nullptr;
std::vector<Slave*> Simulation::entities = {};

Simulation::Simulation() {
    isPaused = true;
}

Simulation::~Simulation() {}

void Simulation::initWindow(const char* title, int x, int y, int width, int height, bool fullscreen) {
    this->fullscreen = fullscreen;
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystems initialized!..." << std::endl;

        window = SDL_CreateWindow(title, x, y, width, height, flags);
        if (window) {
            std::cout << "Window created!" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);
            std::cout << "Renderer created!" << std::endl;
        }
    }
    if (TTF_Init() == -1) {
        std::cout << "Error initializing SDL_TTF!" << std::endl;
    }
    font = TTF_OpenFont("assets/oxanium.ttf", defaultFontSize * fontScale);

    // Pause Message
    TTF_Font* tempFont = TTF_OpenFont("assets/oxanium.ttf", defaultFontSize * fontScale * 2);
    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended_Wrapped(
        tempFont,
        "PRESS ENTER TO PLAY",
        textColor,
        screen.w
    );
    pauseMessage = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    pauseMessageRect = {
        ( width - tmpSurface->w ) / 2,
        ( height - tmpSurface->h ) / 2,
        tmpSurface->w,
        tmpSurface->h
    };
    SDL_FreeSurface(tmpSurface);
    TTF_CloseFont(tempFont);

    screen.w = width;
    screen.h = height;
}

void Simulation::start() {
    SDL_RemoveTimer(nextDayTimer);
    
    ClearEntities();
    
    std::cout << "----- Starting Stimulation -----" << std::endl;

    // Print params
    std::cout << "Starting Population of Slaves  : " << startingSlavePopulation << std::endl;
    std::cout << "Starting Population of Frees : " << startingFreePopulation << std::endl;

    // Study configuration
    AddMaster();
    
    currentNumberOfSlaves = 0;
    for (int i = 0; i < startingSlavePopulation; i++) {
        AddEntity();
        Slave* s = *entities.end();
        master->addSlave(s);
    }

    currentNumberOfFree = 0;
    for (int i = 0; i < startingFreePopulation; i++) AddEntity();

    // Days to days
    srand(time(0));
    
    day = 1;
    
    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, "Day 1", textColor);
    dayCounterText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    dayCounterRect = {screen.w - tmpSurface->w - 16, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);

    nextDayTimer = SDL_AddTimer(dayLength, [](Uint32 interval, void* param) {
        Simulation* window = static_cast<Simulation*>(param);
        window->NextDay();
        return interval;
    }, this);

    isRunning = true;
}

void Simulation::update() {
    if (isPaused || entities.size() <= 0) {
        UpdateCounters();
        return;
    }
    
    master->update();
    for (auto e : entities) {
        e->update();
    }

    UpdateCounters();
}

void Simulation::render() {
    if (isPaused) {
        SDL_RenderCopy(renderer, pauseMessage, nullptr, &pauseMessageRect);
        SDL_RenderPresent(renderer);
        return;
    }

    SDL_RenderClear(renderer);

    master->draw();

    for (auto e : entities) {
        e->draw();
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderCopy(renderer, numberOfSlavesText, nullptr, &numberOfSlavesRect);
    SDL_RenderCopy(renderer, numberOfFreeText, nullptr, &numberOfFreeRect);
    SDL_RenderCopy(renderer, dayCounterText, nullptr, &dayCounterRect);

    SDL_RenderPresent(renderer);
}

void Simulation::handleEvents() {
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_q) {
                isRunning = false;
            }
            if (event.key.keysym.sym == SDLK_r) {
                start();
            }
            if (event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_RETURN) {
                isPaused = !isPaused;
            }
            break;
        default:
            break;
    }

}

void Simulation::kill() {
    ClearEntities();

    TTF_CloseFont(font);
    
    SDL_RemoveTimer(nextDayTimer);

    SDL_DestroyTexture(numberOfSlavesText);
    SDL_DestroyTexture(numberOfFreeText);
    SDL_DestroyTexture(dayCounterText);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    std::cout << "Simulation ended on day " << day << "." << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Window Killed!" << std::endl;
}

void Simulation::NextDay() {
    if (isPaused) {
        return;
    }

    // ----- END OF THE DAY -----
    

    // ----- START OF THE NEW DAY -----
    // update day counter
    day++;
    std::string text = "Day " + std::to_string(day);
    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    dayCounterText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    dayCounterRect = {screen.w - tmpSurface->w - 16, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);
}

void Simulation::AddEntity() {
    int x = rand() % (screen.w - Entity::size);
    int y = rand() % (screen.h - Entity::size);

    Slave* s = new Slave(x, y);
    entities.push_back(s);
}

void Simulation::AddMaster() {
    int x = rand() % (screen.w - Entity::size);
    int y = rand() % (screen.h - Entity::size);

    master = new Master(x, y);
}

void Simulation::UpdateSlavesCounter() {
    if (currentNumberOfSlaves == master->getNumberOfSlaves()) {
        return;
    }
    currentNumberOfSlaves = master->getNumberOfSlaves();
    std::string text = "Entities : " + std::to_string(currentNumberOfSlaves);

    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    numberOfSlavesText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    numberOfSlavesRect = {0, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);
}

void Simulation::UpdateFreesCounter() {
    if (currentNumberOfFree == static_cast<int>(entities.size()) - currentNumberOfSlaves) {
        return;
    }
    currentNumberOfFree = static_cast<int>(entities.size()) - currentNumberOfSlaves;
    std::string text = "Entities : " + std::to_string(currentNumberOfFree);

    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    numberOfFreeText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    numberOfFreeRect = {0, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);
}

void Simulation::UpdateCounters() {
    UpdateSlavesCounter();
    UpdateFreesCounter();
}

void Simulation::ClearEntities() {
    for (auto e : entities) {
        free(e);
    }
    entities.clear();
    
    free(master);
    master = nullptr;
}
