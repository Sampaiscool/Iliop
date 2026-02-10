#pragma once
#include <SDL2/SDL.h>
#include "../Other/CombatState.h"

class Enemy {
public:
    Enemy(int hp, int sh, int mana);
    void render(SDL_Renderer* renderer, int winW, int winH);
    CombatState& getState();

private:
    CombatState state;
};
