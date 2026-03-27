#pragma once
#include "../Enemy/Enemy.h"

#define MAX_FLOORS 12

class EnemyFactory {
public:
    static Enemy create(int floor);
    static bool hasMoreEnemies(int floor);
};
