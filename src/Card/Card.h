#pragma once
#include <SDL2/SDL.h>
#include <string>

enum class CardType { Damage, Heal, Shield };

enum class CardEffect { DealDamage, Heal, GainShield};

struct Card {
    int x, y, w, h;
    std::string name;
    CardType type;
    CardEffect effect;
    int value;

    void draw(SDL_Renderer* renderer);
};
