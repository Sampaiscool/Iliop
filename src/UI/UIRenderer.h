#pragma once
#include <SFML/Graphics.hpp>
#include "../Other/CombatState.h"

class UIRenderer {
public:
    void render(sf::RenderWindow& window, const CombatState& state, int winW, int winH, const sf::Font& font);
};

