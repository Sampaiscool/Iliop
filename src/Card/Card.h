#pragma once
#include <string>
#include <SFML/Graphics.hpp>

enum class CardType { Damage, Heal, Shield };
enum class CardEffect { DealDamage, Heal, GainShield };

struct Card {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    std::string name;
    CardType type;
    CardEffect effect;
    int value = 0;
    int cost;

    bool contains(int mx, int my) const {
        return mx >= x && mx <= x + w &&
               my >= y && my <= y + h;
    }

    void draw(sf::RenderWindow& window, const sf::Font& font) const;

};

