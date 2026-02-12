#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../Effects/Effect.h"
#include "../Other/CombatState.h"

enum class CardType { Damage, Heal, Shield };

struct Card {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    std::string name;
    CardType type;
    std::unique_ptr<Effect> effect;
    int value = 0;
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

    void play(CombatState& self, CombatState& target) const {
    if (effect)
        effect->apply(self, target);
    }

    void draw(sf::RenderWindow& window, const sf::Font& font) const;

};
