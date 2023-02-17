#include "Entity.h"
#include "Window.h"
#include "Constants.h"

int Entity::size = 32 * scale;

Entity::Entity() {
    body = {0, 0, size, size};
    alive = true;
    species = 0; // None
}

Entity::Entity(int x, int y, int species) {
    body = {x, y, size, size};
    this->species = species;
    this->alive = true;
}

Entity::~Entity() {}

void Entity::update() {
    body.x += (rand() % 3 - 1) * speed;
    body.y += (rand() % 3 - 1) * speed;

    if (body.x < 0) body.x = 0;
    if (body.x + body.w > Window::screen.w) body.x = Window::screen.w - body.w;
    if (body.y < 0) body.y = 0;
    if (body.y + body.h > Window::screen.h) body.y = Window::screen.h - body.h;
}

void Entity::draw() {
    // content
    SDL_SetRenderDrawColor(Window::renderer, color[species].r, color[species].g, color[species].b, color[species].a);
    SDL_RenderFillRect(Window::renderer, &body);
    // border
    SDL_SetRenderDrawColor(Window::renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(Window::renderer, &body);
}
