#pragma once
#include "Entity.h"

class Slave : public Entity {
private:
    /* data */
public:
    Slave(int x, int y);
    ~Slave();
    void update() override;
};
