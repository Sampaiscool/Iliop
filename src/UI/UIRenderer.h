#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <vector>
#include "../Other/CombatState.h"
#include "../Card/Card.h"
#include "../Other/Status.h"
#include "../Character/Character.h"

class UIRenderer {
public:
    void render(sf::RenderWindow& window, const Character& player, const CombatState& playerState, const CombatState& enemyState, int winW, int winH, const sf::Font& font);
    void drawTooltip(sf::RenderWindow& window, const sf::Font& font, const Card& card, float mouseX, float mouseY);

    void loadStatusTextures();
    void drawStatusTooltip(sf::RenderWindow& window, const sf::Font& font, const Status& status, sf::Vector2f mousePos);

    const std::map<StatusType, sf::Texture>& getTextures() const { return statusTextures; }

    void loadPlayerTextures() {
        (void)portraitTextures[CharacterName::Hiroshi].loadFromFile("../assets/portraits/HiroshiPortrait.png");
        (void)portraitTextures[CharacterName::Phlox].loadFromFile("../assets/portraits/PhloxPortrait.png");
        (void)portraitTextures[CharacterName::MightyFire].loadFromFile("../assets/portraits/MightyFire.png");
    }

    sf::FloatRect getEndTurnBounds() const;
    sf::FloatRect getTransformBounds() const;
private:
    std::map<CharacterName, sf::Texture> portraitTextures;
    std::map<StatusType, sf::Texture> statusTextures;
    sf::FloatRect endTurnBounds;
    sf::FloatRect transformBounds;
};

