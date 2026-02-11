#pragma once
#include <SFML/Graphics.hpp>
#include "../Other/CombatState.h"

enum class EnemyType { MisterEraser, Menta };

class Enemy {
public:
    Enemy(int hp, int sh, int mana, int cor, EnemyType chosenType);
    void render(sf::RenderWindow& window, int winW, int winH);
    CombatState& getState();
    void playTurn(CombatState& playerState);
    void rollIntent();
    int getIntentDamage() const;
    int getIntentBlock() const;

private:
    CombatState state;
    EnemyType type;
    int intendedDamage = 0;
    int intendedBlock  = 0;

};
