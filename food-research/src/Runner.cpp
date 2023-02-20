#include "Runner.h"
#include "Constants.h"

Runner::Runner(int x, int y) : Entity(x,y) {
    color = &runnerColor;
    speed = 4;
}

Runner::~Runner() {}
