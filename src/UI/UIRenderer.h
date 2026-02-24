#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include "../Other/CombatState.h"
#include "../Card/Card.h"
#include "../Other/Status.h"
#include "../Character/Character.h"

struct FloatingText {
    sf::Text text;
    sf::Vector2f position;
    sf::Color color;
    float lifetime = 0.5f;
};

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
        (void)portraitTextures[CharacterName::Vortex].loadFromFile("../assets/portraits/MightyFire.png"); //TODO Fix this to vortex
    }

    void spawnFCT(sf::Vector2f pos, std::string str, sf::Color color, const sf::Font& font);
    void updateAndDrawFCT(sf::RenderWindow& window, float dt);
    void resetHPTracking();

    sf::FloatRect getEndTurnBounds() const;
    sf::FloatRect getTransformBounds() const;
private:
    std::vector<FloatingText> floatingTexts;
    std::map<CharacterName, sf::Texture> portraitTextures;
    std::map<StatusType, sf::Texture> statusTextures;
    sf::FloatRect endTurnBounds;
    sf::FloatRect transformBounds;
    int lastPlayerHP = -1;
    int lastEnemyHP = -1;
    sf::Vector2f lastPlayerBarPos;
    sf::Vector2f lastEnemyBarPos;
};

