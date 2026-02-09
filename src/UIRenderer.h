#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "CombatState.h"

class UIRenderer {
public:
    void render(SDL_Renderer* renderer, const CombatState& state, int winW, int winH, TTF_Font* font);
};
