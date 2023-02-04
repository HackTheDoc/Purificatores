#include "Window.h"

const int direction[2] = {-1, 1};

bool Window::isRunning = false;

Window::Window() {
    window = nullptr;
    renderer = nullptr;
    fullscreen = false;
    screen = {0, 0, 0, 0};
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
    screen.w = width;
    screen.h = height;
    this->width = width/(caseSize * scale);
    this->height = height/(caseSize * scale);

    // init colors
    color[WHITE] =  {255, 255, 255, 255};
    color[PURPLE] = {153,   0, 153, 255};
    color[PINK] =   {255, 153, 204, 255};
    color[RED] =    {255,   0,   0, 255};
    color[GREEN] =  {102, 255, 102, 255};
    color[BLUE] =   {  0, 102, 204, 255};

    start();
}

void Window::update() {
    if (isPaused) {
        return;
    }

    // Propagation algorithm
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Unit* attacker = units[y * width + x];
            if (attacker->beenConqueredThisTurn) {
                attacker->beenConqueredThisTurn = false;
            }
            else if (attacker->id != WHITE) {
                unitAttack(attacker);             
            }
        }
    }
}

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

    if (isPaused) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
        SDL_RenderFillRect(renderer, &screen);
    }
    
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
            if (event.key.keysym.sym == SDLK_p) {
                isPaused = !isPaused;
            }
            if (event.key.keysym.sym == SDLK_r) {
                start();
            }
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

void Window::start() {
    // init units to WHITE units
    units = (Unit**)malloc(this->height * this->width * sizeof(Unit));
    for (int y = 0; y < this->height; y++) {
        for (int x = 0; x < this->width; x++) {
            units[y * this->width + x] = new Unit();
            units[y * this->width + x]->rect = {x*caseSize*scale, y*caseSize*scale, caseSize*scale, caseSize*scale};
        }
    }

    // spawn 4 differents units at each corner
    units[0]->id = PURPLE;
    units[this->width -1]->id = GREEN;
    units[ (this->height-1)*this->width ]->id = BLUE;
    units[ this->height*this->width -1]->id = PINK;
}

void Window::unitAttack(Unit* attacker) {
    int pos[2];
    pos[0] = attacker->rect.x / (caseSize*scale);
    pos[1] = attacker->rect.y / (caseSize*scale);

    pos[rand()%2] += direction[rand()%2];
    if (
        pos[0] >= 0    &&
        pos[0] < width &&
        pos[1] >= 0    &&
        pos[1] < height 
    ) {
        Unit* target = units[pos[1] * width + pos[0]];
        if (target->id != attacker->id) {
            target->id = attacker->id;
            target->beenConqueredThisTurn = true;
        }
    }
}
