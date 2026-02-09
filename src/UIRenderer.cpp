#include "UIRenderer.h"

static void drawBar(
    SDL_Renderer* renderer,
    int x, int y, int w, int h,
    int current, int max,
    SDL_Color fill,
    SDL_Color back)
{
    // Background
    SDL_Rect bg { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, back.r, back.g, back.b, 255);
    SDL_RenderFillRect(renderer, &bg);

    // Fill
    float pct = (float)current / (float)max;
    SDL_Rect fg { x, y, (int)(w * pct), h };
    SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, 255);
    SDL_RenderFillRect(renderer, &fg);

    // Border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &bg);
}

void UIRenderer::render(SDL_Renderer* renderer, const CombatState& state, int winW, int winH) {
    int barWidth  = winW / 4;
    int barHeight = winH / 25;
    int margin    = winW / 40;

    int x = winW - barWidth - margin;
    int y = margin;

    // HP bar
    drawBar(
        renderer,
        x,
        y,
        barWidth,
        barHeight,
        state.hp.current,
        state.hp.max,
        {200, 50, 50, 255},
        {60, 20, 20, 255}
    );

    // mana bar
    drawBar(
        renderer,
        x,
        y + barHeight + margin / 2,
        barWidth,
        barHeight,
        state.mana.current,
        state.mana.max,
        {50, 50, 200, 255},
        {20, 20, 60, 255}
    );

}
