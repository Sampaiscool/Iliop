#pragma once
#include <SFML/Graphics.hpp>
#include "../Other/CombatState.h"

class Enemy {
public:
    Enemy(int hp, int sh, int mana, int cor);
    void render(sf::RenderWindow& window, int winW, int winH);
    CombatState& getState();

private:
    CombatState state;
};
