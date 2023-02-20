#pragma once
#include <vector>
#include "Entity.h"
#include "Slave.h"

class Master : public Entity {
private:
    std::vector<Slave*> slaves;

public:
    static int size;

    Master(int x, int y);
    ~Master();
    void update() override;
    void addSlave(Slave* s);
    int getNumberOfSlaves();
};