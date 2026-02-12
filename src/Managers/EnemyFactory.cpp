#include "EnemyFactory.h"

Enemy EnemyFactory::create(int floor)
{
    // hp - shield - mana - Type
    if (floor < 3) {
        return Enemy(12, 5, 1, 10, EnemyType::MisterEraser);
    }
    else if (floor < 6) {
        return Enemy(18, 8, 1, 10, EnemyType::Menta);
    }
    else {
        return Enemy(30, 10, 2, 15, EnemyType::CursedKing);
    }
}
