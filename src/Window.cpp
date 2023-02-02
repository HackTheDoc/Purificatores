#include "Window.h"

bool Window::isRunning = false;

Window::Window() {
    window = nullptr;
    renderer = nullptr;
    fullscreen = false;
}

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
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created!" << std::endl;
        }
        isRunning = true;
    }
    this->width = width/(caseSize * scale);
    this->height = height/(caseSize * scale);

    // init colors
    color[WHITE] =  {255, 255, 255, 255};
    color[PURPLE] = {153,   0, 153, 255};
    color[PINK] =   {255, 153, 204, 255};
    color[RED] =    {255,   0,   0, 255};
    color[GREEN] =  {  0, 255,   0, 255};
    color[BLUE] =   {  0,   0, 255, 255};

    // init units to WHITE units
    units = (Unit**)malloc(this->height * this->width * sizeof(Unit));
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            units[y * this->width + x] = new Unit();
            units[y * this->width + x]->id = WHITE;
            units[y * this->width + x]->rect = {x*caseSize*scale, y*caseSize*scale, caseSize*scale, caseSize*scale};
        }
    }
    units[0]->id = PURPLE;
}

void Window::update() {}

void Window::render() {
    SDL_RenderClear(renderer);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = units[y * width + x]->id;
            SDL_Rect* r = &units[y * width + x]->rect;

            SDL_SetRenderDrawColor(renderer, color[i].r, color[i].g, color[i].b, color[i].a);
            SDL_RenderFillRect(renderer, r);
        }
    }
    
    SDL_RenderPresent(renderer);
}

void Window::handleEvents() {
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }

}

void Window::kill() {
    free(units);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Window Killed!" << std::endl;
}
