#pragma once
#include <SFML/Graphics.hpp>
#include "../Other/CombatState.h"
#include "../Card/Card.h"

class UIRenderer {
public:
    void render(sf::RenderWindow& window, const CombatState& playerState, const CombatState& enemyState, int winW, int winH, const sf::Font& font);
    void drawTooltip(sf::RenderWindow& window, const sf::Font& font, const Card& card, float mouseX, float mouseY);

    sf::FloatRect getEndTurnBounds() const;
    sf::FloatRect getTransformBounds() const;
private:
    sf::FloatRect endTurnBounds;
    sf::FloatRect transformBounds;
};

