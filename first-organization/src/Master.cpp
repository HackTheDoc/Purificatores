#include "Master.h"
#include "Simulation.h"
#include "Constants.h"

int Master::size = Entity::size * 2;

Master::Master(int x, int y) {
    body = {x, y, size, size};
    speed = 2;
    color = &masterColor;
    destinationX = Simulation::screen.w / 2;
    destinationY = Simulation::screen.h / 2;
}

Master::~Master() {}

void Master::update() {
    moveTowardsDestination();
}

void Master::addSlave(Slave* s) {
    slaves.emplace_back(s);
}

int Master::getNumberOfSlaves() {
    return static_cast<int>(slaves.size());
}
