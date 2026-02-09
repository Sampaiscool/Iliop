#pragma once
#include <SDL2/SDL.h>

enum class CardType { Damage, Heal, Shield };

struct Card {
    int x, y, w, h;
    CardType type;
    int value;

    void draw(SDL_Renderer* renderer);
};
