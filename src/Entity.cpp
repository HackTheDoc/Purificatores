#include "Entity.h"
#include "Window.h"
#include "Constants.h"

int Entity::size = 32 * scale;

double getDistance(int x1, int y1, int x2, int y2) {
    return sqrt( pow(x1 - x2, 2) + pow(y1 - y2, 2) );
}

Entity::Entity() {
    body = {0, 0, size, size};
    destinationX = destinationY = 0;
    alive = true;
    species = 0; // None
    reproducing = 0;
}

Entity::Entity(int x, int y, int species) {
    body = {x, y, size, size};
    destinationX = x;
    destinationY = y;
    this->species = species;
    this->alive = true;
    reproducing = 0;
}

Entity::~Entity() {}

void Entity::update() {
    double distance = getDistance(destinationX, destinationY, body.x, body.y);
    
    if (distance <= body.w/2 || distance <= body.h/2) {
        // pick a new destination
        do {
            destinationX = body.x + ( rand() % (body.w * 3) ) * ( rand() % 3 - 1 );
            destinationY = body.y + ( rand() % (body.h * 3) ) * ( rand() % 3 - 1 );
        } while (
            destinationX < 0 || destinationX > Window::screen.w - body.w ||
            destinationY < 0 || destinationY > Window::screen.h - body.h
        );
        return;
    }
    
    // move to the destination
    double fraction = speed / distance;

    body.x += (int)((destinationX - body.x) * fraction);
    body.y += (int)((destinationY - body.y) * fraction);
    
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

void Entity::reproduce() {
    if (reproducing) {
        reproducing--;
        return;
    }

    for (auto partner : Window::entities) {
        double d = getDistance(body.x, body.y, partner->body.x, partner->body.y);
        
        if (d > size * 1.5 || this == partner) {
            reproducing--;
            return;
        }

        partner->reproducing = 2;
        this->reproducing = 2;

        // did they fucked ?
        double r = (double)rand() / RAND_MAX;
        if (r <= reproducingChance) {
            Window::AddEntity();
        }
    }
}
