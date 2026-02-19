#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../Effects/Effect.h"
#include "../Other/CombatState.h"

enum class CardType { 
    Damage,
    Heal,
    Shield,
    PrimalArrow
};

enum class CardTheme {
    Red, Blue, Green, Purple, Gold, Gray, Brown, Teal
};

struct Card {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    std::string name;
    std::string description;
    CardType type;
    CardTheme theme;
    std::unique_ptr<Effect> effect;
    int value = 0;
    int corruptedValue = 0;
    int cost;

    Card() = default;

    // forbid copy
    Card(const Card&) = delete;
    Card& operator=(const Card&) = delete;

    // allow move
    Card(Card&&) noexcept = default;
    Card& operator=(Card&&) noexcept = default;

    bool contains(int mx, int my) const {
        return mx >= x && mx <= x + w &&
               my >= y && my <= y + h;
    }

    void play(CombatState& self, CombatState& target, bool isCorrupted) const {
        if (effect) {
            // THIS is where the addition happens
            int totalPower = value; 
            if (isCorrupted) {
                totalPower += corruptedValue;
            }

            // Now we pass the SUM. 
            // Because totalPower is now > 0, the Effect will use this 
            // instead of its internal 'amount'.
            effect->apply(self, target, totalPower);
        }
    }

    void draw(sf::RenderWindow& window, const sf::Font& font, bool isCorrupted, const CombatState& playerState) const;

    int getActiveValue(bool isCorrupted) const {
        // if you are corrupted add the bonus, else not
        return isCorrupted ? (value + corruptedValue) : value;
    }

    sf::FloatRect getBounds() const {
        return sf::FloatRect({(float)x, (float)y}, {(float)w, (float)h});
    }
};
