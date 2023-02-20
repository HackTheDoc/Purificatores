#include "Entity.h"
#include "Simulation.h"
#include "Constants.h"
#include <algorithm>

int Entity::size = entityDefaultSize * scale;

double distance(int x1, int y1, int x2, int y2) {
    return sqrt( pow(x1 - x2, 2) + pow(y1 - y2, 2) );
}

Entity::Entity(int x, int y) {
    body = {x, y, size, size};
    alive = true;
    hunger = 0;
    targetedFood = nullptr;
}

Entity::~Entity() {}

void Entity::update() {
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

void Entity::draw() {
    // content
    SDL_SetRenderDrawColor(Simulation::renderer, entityColor.r, entityColor.g, entityColor.b, entityColor.a);
    SDL_RenderFillRect(Simulation::renderer, &body);
    // border
    SDL_SetRenderDrawColor(Simulation::renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    SDL_RenderDrawRect(Simulation::renderer, &body);
}

void Entity::findNearestFoodSource() {
    targetedFood = nullptr;
    double minDistance = INFINITY;

    for (auto f : Simulation::foodSources) {
        double dist = distance(body.x, body.y, f->x, f->y);
        if (dist < minDistance) {
            targetedFood = f;
            minDistance = dist;
        }
    }

    // set destination depending of targeted food
    if (hunger && targetedFood != nullptr) {
        destinationX = targetedFood->x;
        destinationY = targetedFood->y;
    } else {
        destinationX = body.x;
        destinationY = body.y;
    }
}

void Entity::eat() {
    if (targetedFood == nullptr) {
        return;
    }

    double dist = distance(destinationX, destinationY, body.x, body.y);
    if (dist >= size/2) {
        return;
    }

    auto it = std::find(Simulation::foodSources.begin(), Simulation::foodSources.end(), targetedFood);
    if (it != Simulation::foodSources.end()) {
        // let's eat !
        hunger -= 25;
        if (hunger < 0) hunger = 0;

        // and remove the eaten food source of the map xD
        Simulation::foodSources.erase(it);
        delete targetedFood;
        targetedFood = nullptr;
    } else {
        findNearestFoodSource();
    }
}

void Entity::starve() {
    hunger += 25;
    if (hunger > 100) {
        alive = false;
    }
}
