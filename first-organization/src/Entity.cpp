#include "Entity.h"
#include "Simulation.h"
#include "Constants.h"
#include <algorithm>

int Entity::size = entityDefaultSize * scale;

double distance(int x1, int y1, int x2, int y2) {
    return sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) );
}

Entity::Entity() {
    body = {0,0,0,0};
    color = &entityColor;
    speed = 0;
}

Entity::Entity(int x, int y) {
    body = {x, y, size, size};
    color = &entityColor;
    speed = 1;
}

Entity::~Entity() {}

void Entity::draw() {
    // content
    SDL_SetRenderDrawColor(Simulation::renderer, color->r, color->g, color->b, color->a);
    SDL_RenderFillRect(Simulation::renderer, &body);
    // border
    SDL_SetRenderDrawColor(Simulation::renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(Simulation::renderer, &body);
}

void Entity::moveTowardsDestination() {
    double dist = distance(destinationX, destinationY, body.x, body.y);
    
    if (dist <= size/2) {
        return;
    }
    
    // move to the destination
    double fraction = speed / dist;

    body.x += (int)((destinationX - body.x) * fraction);
    body.y += (int)((destinationY - body.y) * fraction);
    
    if (body.x < 0) body.x = 0;
    if (body.x + body.w > Simulation::screen.w) body.x = Simulation::screen.w - body.w;
    if (body.y < 0) body.y = 0;
    if (body.y + body.h > Simulation::screen.h) body.y = Simulation::screen.h - body.h;
}

