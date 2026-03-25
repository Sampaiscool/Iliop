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
    float lifetime;
};

class UIRenderer {
public:
    void render(sf::RenderWindow& window,
                const Character& player,
                const CombatState& playerState,
                const CombatState& enemyState,
                int winW,
                int winH,
                const sf::Font& font,
                const std::string& enemyIntentDescription);
    void drawTooltip(sf::RenderWindow& window, const sf::Font& font, const Card& card, float mouseX, float mouseY);

    void loadStatusTextures();
    void drawStatusTooltip(sf::RenderWindow& window, const sf::Font& font, const Status& status, sf::Vector2f mousePos);

    const std::map<StatusType, sf::Texture>& getTextures() const { return statusTextures; }

    void loadPlayerTextures() {
        (void)portraitTextures[CharacterName::Hiroshi].loadFromFile("../assets/portraits/HiroshiPortrait.png");
        (void)portraitTextures[CharacterName::Phlox].loadFromFile("../assets/portraits/PhloxPortrait.png");
        (void)portraitTextures[CharacterName::Vortex].loadFromFile("../assets/portraits/VortexPortrait.png");
        (void)portraitTextures[CharacterName::OneXNAO].loadFromFile("../assets/portraits/1XNAOPortrait.png");
        (void)portraitTextures[CharacterName::Roshka].loadFromFile("../assets/portraits/RoshkaPortrait.png");
    }

    void spawnFCT(sf::Vector2f pos, std::string str, sf::Color color, const sf::Font& font, int charSize = 24);
    void updateAndDrawFCT(sf::RenderWindow& window, float dt);
    void resetHPTracking();

    sf::FloatRect getEndTurnBounds() const;
    sf::FloatRect getTransformBounds() const;

    void scrollPlayerStatuses(int delta);
    void scrollEnemyStatuses(int delta);
    void resetStatusScrolls();

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
    int playerStatusScroll = 0;
    int enemyStatusScroll = 0;
};

