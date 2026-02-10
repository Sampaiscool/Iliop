#pragma once
#include <string>

enum class CardType { Damage, Heal, Shield };
enum class CardEffect { DealDamage, Heal, GainShield };

struct Card {
    int x, y, w, h;
    std::string name;
    CardType type;
    CardEffect effect;
    int value;

    bool contains(int mx, int my) const {
        return mx >= x && mx <= x + w &&
               my >= y && my <= y + h;
    }
};
