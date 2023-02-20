#include "Entity.h"
#include "Simulation.h"
#include "Constants.h"
#include <algorithm>

int Entity::size = entityDefaultSize * scale;

double distance(int x1, int y1, int x2, int y2) {
    return sqrt( pow(x2 - x1, 2) + pow(y2 - y1, 2) );
}

Entity::Entity(int x, int y) {
    body = {x, y, size, size};
    color = &entityColor;
    alive = true;
    hunger = 10;
    targetedFood = nullptr;
    speed = 1;
    hungerRisingRate = 1;
}

Entity::~Entity() {}

void Entity::update() {
    moveTowardsDestination();
    
    if (hunger > 20000) {   // 1000*10*4, entity die if he doesn't eat for 2 days
        alive = false;
    }
}

void Entity::draw() {
    // content
    SDL_SetRenderDrawColor(Simulation::renderer, color->r, color->g, color->b, color->a);
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
    if (targetedFood != nullptr) {
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

    auto it = std::find(Simulation::foodSources.begin(), Simulation::foodSources.end(), targetedFood);
    if (it != Simulation::foodSources.end()) {
        double dist = distance(destinationX, destinationY, body.x, body.y);
        if (dist <= size/2) {
            // let's eat !
            hunger -= 2000;
            if (hunger < 0) hunger = 0;

            // and remove the eaten food source of the map xD
            Simulation::foodSources.erase(it);
            delete targetedFood;
            targetedFood = nullptr;

            if (hunger) {
                findNearestFoodSource();
            }
        }
    } else {
        findNearestFoodSource();
    }
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

    // when moving, the entity hunger rise up
    hunger += hungerRisingRate;
}
