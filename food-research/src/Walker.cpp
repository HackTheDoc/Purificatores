#include "Walker.h"
#include "Constants.h"

Walker::Walker(int x, int y) : Entity(x, y) {
    color = &walkerColor;
    speed = 2;
}

Walker::~Walker() {}
