#include "Card.h"

void Card::draw(SDL_Renderer* renderer) {
    SDL_Rect rect{ x, y, w, h };

    // set correct data based on cardtype
    switch (type) {
        case CardType::Damage:
            SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
            break;
        case CardType::Heal:
            SDL_SetRenderDrawColor(renderer, 50, 200, 50, 255);
            break;
        case CardType::Shield:
            SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
            break;
    }

    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

