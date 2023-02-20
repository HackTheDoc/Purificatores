#include "Simulation.h"
#include <cstdlib>
#include <ctime>


SDL_Renderer* Simulation::renderer = nullptr;
SDL_Rect Simulation::screen = {0, 0, 0, 0};
bool Simulation::isRunning = false;
std::vector<Entity*> Simulation::entities = {};
std::vector<Food*> Simulation::foodSources = {};

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
    ClearFoodSources();
    
    std::cout << "----- Starting Stimulation -----" << std::endl;

    // Print params
    std::cout << "Starting Population   : " << startingPopulation << std::endl;
    std::cout << "Starting Food Sources : " << startingFoodSources << std::endl;

    // Study configuration
    currentNumberOfEntities = 0;
    for (int i = 0; i < startingPopulation; i++) AddEntity();

    currentNumberOfFoodSources = startingFoodSources;
    for (int i = 0; i < currentNumberOfFoodSources; i++) AddFoodSource();

    for (auto e : entities) {
        e->starve();
        e->findNearestFoodSource();
    }

    // Days to days
    srand(time(0));
    
    day = 1;
    
    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, "Day 1", textColor);
    dayCounterText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    dayCounterRect = {screen.w - tmpSurface->w - 16, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);

    nextDayTimer = SDL_AddTimer(1000*10, [](Uint32 interval, void* param) {
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
    
    for (auto e : entities) {
        e->update();
        e->eat();
    }

    for (auto f : foodSources) {
        f->update();
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

    for (auto f : foodSources) {
        f->draw();
    }
    for (auto e : entities) {
        e->draw();
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderCopy(renderer, numberOfEntitiesText, nullptr, &numberOfEntitiesRect);
    SDL_RenderCopy(renderer, numberOfFoodSourcesText, nullptr, &numberOfFoodSourcesRect);
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
    ClearFoodSources();

    TTF_CloseFont(font);
    
    SDL_RemoveTimer(nextDayTimer);

    SDL_DestroyTexture(numberOfEntitiesText);
    SDL_DestroyTexture(numberOfFoodSourcesText);
    SDL_DestroyTexture(dayCounterText);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    std::cout << "Final number of living entities : " << currentNumberOfEntities << std::endl;
    std::cout << "Window Killed!" << std::endl;
}

void Simulation::NextDay() {
    if (isPaused) {
        return;
    }

    // ----- END OF THE DAY -----
    // Dead entities ?
    auto itEntity = entities.begin();
    while (itEntity != entities.end()) {
        Entity* e = *itEntity;
        e->starve();

        if (e->alive) {
            itEntity++;
        } else {
            entities.erase(itEntity);
            delete e;
        }
    }

    // Expiring food sources
    auto itFood = foodSources.begin();
    while (itFood != foodSources.end()) {
        Food* f = *itFood;

        if (f->expired) {
            foodSources.erase(itFood);
            delete f;
        } else {
            itFood++;
        }
    }

    // ----- START OF THE NEW DAY -----
    // update day counter
    day++;
    std::string text = "Day " + std::to_string(day);
    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    dayCounterText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    dayCounterRect = {screen.w - tmpSurface->w - 16, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);

    // spawn new food source relatively to current day
    int amount = startingFoodSources * pow(foodSourcesRarificationRate, day);
    for (int i = 0; i < amount; i++) AddFoodSource();

    // search for new food source
    for (auto e : entities) {
        e->findNearestFoodSource();
    }
}

void Simulation::AddEntity() {
    int x = rand() % (screen.w - Entity::size);
    int y = rand() % (screen.h - Entity::size);

    if (rand() % 2) {
        Runner* e = new Runner(x, y);
        entities.push_back(e);
    } else {
        Walker* e = new Walker(x, y);
        entities.push_back(e);
    }
}

void Simulation::AddFoodSource() {
    int x = rand() % (screen.w - Food::size);
    int y = rand() % (screen.h - Food::size);
    Food* f = new Food(x, y);
    foodSources.push_back(f);
}

void Simulation::UpdateEntitiesCounter() {
    if (currentNumberOfEntities == static_cast<int>(entities.size())) {
        return;
    }
    currentNumberOfEntities = static_cast<int>(entities.size());
    std::string text = "Entities : " + std::to_string(currentNumberOfEntities);

    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    numberOfEntitiesText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    numberOfEntitiesRect = {0, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);
}

void Simulation::UpdateFoodSourcesCounter() {
    if (currentNumberOfFoodSources == static_cast<int>(foodSources.size())) {
        return;
    }
    currentNumberOfFoodSources = static_cast<int>(foodSources.size());
    std::string text = "Food Sources : " + std::to_string(currentNumberOfFoodSources);

    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    numberOfFoodSourcesText = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    numberOfFoodSourcesRect = {0, numberOfEntitiesRect.y + numberOfEntitiesRect.h + 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);
}

void Simulation::UpdateCounters() {
    UpdateEntitiesCounter();
    UpdateFoodSourcesCounter();
}

void Simulation::ClearEntities() {
    for (auto e : entities) {
        free(e);
    }
    entities.clear();
}

void Simulation::ClearFoodSources() {
    for (auto f : foodSources) {
        free(f);
    }
    foodSources.clear();
}
