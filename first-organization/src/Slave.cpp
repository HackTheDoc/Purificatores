#include "Slave.h"
#include "Constants.h"

Slave::Slave(int x, int y) : Entity(x, y) {
    speed = 4;
    color = &slaveColor;
}

Slave::~Slave() {}

void Slave::update() {

}
