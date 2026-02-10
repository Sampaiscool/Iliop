#include "Enemy.h"

Enemy::Enemy(int hp, int sh, int mana) {
    state = {
        {hp, hp},     // hp
        {0, sh},     // shield
        {mana, mana}  // mana
    };
}

void Enemy::render(SDL_Renderer* renderer, int winW, int winH) {
    int w = winW / 4;
    int h = winH / 4;
    int x = winW / 2 - (w / 2);
    int y = winH / 4;

    SDL_Color fill = {255, 0, 255, 255};     // fill
    SDL_Color back = {0, 0, 0, 255};         // border

    // background
    SDL_Rect bg { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, back.r, back.g, back.b, 255);
    SDL_RenderFillRect(renderer, &bg);

    // fill
    SDL_Rect fg { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, 255);
    SDL_RenderFillRect(renderer, &fg);

    // border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bg);
}

CombatState& Enemy::getState() {
    return state;
}
