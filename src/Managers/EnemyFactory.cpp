#include "EnemyFactory.h"
#include <random>

Enemy EnemyFactory::create(int floor)
{
    std::vector<EnemyType> pool;

    if (floor <= 3) {
        pool = {EnemyType::MisterEraser, EnemyType::LittleGremlin, EnemyType::AngerBot, EnemyType::Puth, EnemyType::WarChief};
    } else if (floor <= 6) {
        pool = {EnemyType::FatGremlin, EnemyType::Bungus, EnemyType::CultistMember, EnemyType::GiggyGrass, EnemyType::BulletJunior};
    } else if (floor <= 9) {
        pool = {EnemyType::MagicSpright, EnemyType::Lihm, EnemyType::Nutou, EnemyType::MushroomMan, EnemyType::Biyumi};
    } else {
        pool = {EnemyType::FatedTraveler, EnemyType::Kazan, EnemyType::DeathMarcher, EnemyType::Menta, EnemyType::CursedKing};
    }
    // pick enemy from floor
    int idx = rand() % pool.size();
    EnemyType chosenType = pool[idx];

    // set stats based on floor
    int hp = 15;
    int shield = 5;
    int mana = 1;
    int corruption = 10;

    // scale with floor
    float scale = 1.0f + (floor * 0.15f);

    switch (chosenType) {
        case EnemyType::MisterEraser:
            hp = 12; shield = 5; mana = 1; corruption = 10;
            break;
        case EnemyType::LittleGremlin:
            hp = 8; shield = 3; mana = 1; corruption = 8;
            break;
        case EnemyType::AngerBot:
            hp = 15; shield = 2; mana = 1; corruption = 10;
            break;
        case EnemyType::Puth:
            hp = 10; shield = 4; mana = 1; corruption = 10;
            break;
        case EnemyType::WarChief:
            hp = 18; shield = 5; mana = 1; corruption = 12;
            break;
        case EnemyType::FatGremlin:
            hp = 20; shield = 5; mana = 1; corruption = 12;
            break;
        case EnemyType::Bungus:
            hp = 18; shield = 8; mana = 1; corruption = 10;
            break;
        case EnemyType::CultistMember:
            hp = 16; shield = 4; mana = 2; corruption = 15;
            break;
        case EnemyType::GiggyGrass:
            hp = 14; shield = 3; mana = 1; corruption = 10;
            break;
        case EnemyType::BulletJunior:
            hp = 20; shield = 6; mana = 1; corruption = 12;
            break;
        case EnemyType::MagicSpright:
            hp = 30; shield = 0; mana = 2; corruption = 12;
            break;
        case EnemyType::Lihm:
            hp = 30; shield = 7; mana = 1; corruption = 10;
            break;
        case EnemyType::Nutou:
            hp = 30; shield = 2; mana = 2; corruption = 12;
            break;
        case EnemyType::MushroomMan:
            hp = 25; shield = 5; mana = 1; corruption = 10;
            break;
        case EnemyType::FatedTraveler:
            hp = 40; shield = 5; mana = 2; corruption = 15;
            break;
        case EnemyType::Kazan:
            hp = 10; shield = 10; mana = 2; corruption = 15;
            break;
        case EnemyType::DeathMarcher:
            hp = 50; shield = 8; mana = 2; corruption = 20;
            break;
        case EnemyType::Biyumi:
            hp = 40; shield = 5; mana = 3; corruption = 25;
            break;
        case EnemyType::Menta:
            hp = 20; shield = 8; mana = 2; corruption = 15;
            break;
        case EnemyType::CursedKing:
            hp = 30; shield = 10; mana = 2; corruption = 15;
            break;
        default:
            hp = 15; shield = 5; mana = 1; corruption = 10;
            break;
    }
    // apply floor scaling
    hp = (int)(hp * scale);
    shield = (int)(shield * scale);

    return Enemy(hp, shield, mana, corruption, chosenType);
}
