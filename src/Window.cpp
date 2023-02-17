#include "Window.h"
#include <cstdlib>
#include <ctime>


SDL_Renderer* Window::renderer = nullptr;
SDL_Rect Window::screen = {0, 0, 0, 0};
bool Window::isRunning = false;

Window::Window() {}

Window::~Window() {}

void Window::init(const char* title, int x, int y, int width, int height, bool fullscreen) {
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
        isRunning = true;
    }
    if (TTF_Init() == -1) {
        std::cout << "Error initializing SDL_TTF!" << std::endl;
    }
    font = TTF_OpenFont("assets/oxanium.ttf", defaultFontSize * fontScale);

    TTF_Font* tempFont = TTF_OpenFont("assets/oxanium.ttf", defaultFontSize * fontScale * 2);
    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended_Wrapped(
        tempFont,
        "PRESS ENTER TO PLAY",
        textColor,
        screen.w
    );
    pauseMessage = SDL_CreateTextureFromSurface(Window::renderer, tmpSurface);
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

    isPaused = true;

    studiedSpecies = SPECIES_1;
    currentNumberOfEntities = 0;

    srand(time(0));

    nextDayTimer = SDL_AddTimer(500, [](Uint32 interval, void* param) {
        Window* window = static_cast<Window*>(param);
        window->NextDay();
        return interval;
    }, this);
}

void Window::update() {
    if (isPaused ) {
        return;
    }

    for (auto e : entities) {
        e->update();
    }

    if (currentNumberOfEntities == static_cast<int>(entities.size())) {
        return;
    }
    currentNumberOfEntities = static_cast<int>(entities.size());
    std::string text = speciesName.at(studiedSpecies) + " : " + std::to_string(currentNumberOfEntities);

    SDL_Surface* tmpSurface;
    tmpSurface = TTF_RenderText_Blended(Window::font, text.c_str(), textColor);
    studyData = SDL_CreateTextureFromSurface(Window::renderer, tmpSurface);
    studyDataRect = {0, 16, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);
}

void Window::render() {
    if (isPaused) {
        SDL_RenderCopy(renderer, pauseMessage, nullptr, &pauseMessageRect);
        SDL_RenderPresent(renderer);
        return;
    }

    SDL_RenderClear(renderer);

    for (auto e : entities) {
        e->draw();
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderCopy(renderer, studyData, nullptr, &studyDataRect);

    SDL_RenderPresent(renderer);
}

void Window::handleEvents() {
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_q) {
                isRunning = false;
            }
            if (event.key.keysym.sym == SDLK_p || event.key.keysym.sym == SDLK_RETURN) {
                isPaused = !isPaused;
            }
            break;
        default:
            break;
    }

}

void Window::kill() {
    for (auto e : entities) {
        free(e);
    }
    TTF_CloseFont(font);
    
    SDL_RemoveTimer(nextDayTimer);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Window Killed!" << std::endl;
}

void Window::NextDay() {
    if (isPaused) {
        return;
    }

    double r;

    // Death ?
    auto it = entities.begin();
    while (it != entities.end()) {
        Entity* e = *it;

        r = (double)rand() / RAND_MAX;
        if (r <= deathChance) {
            entities.erase(it);
            delete e;
        } else {
            it++;
        }
    }
    
    // Birth ?
    r = (double)rand() / RAND_MAX;
    if (r <= birthChance) {
        int x = rand() % (screen.w - Entity::size);
        int y = rand() % (screen.h - Entity::size);
        Entity* e = new Entity(x, y, studiedSpecies);
        entities.push_back(e);
    }
}
