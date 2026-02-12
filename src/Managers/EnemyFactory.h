#pragma once
#include "../Enemy/Enemy.h"

class EnemyFactory {
public:
    static Enemy create(int floor);
};
