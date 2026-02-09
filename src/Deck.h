#pragma once
#include <vector>
#include "Card.h"

class Deck {
public:
    Deck();
    void shuffle();
    void drawInitialHand();
    void render(SDL_Renderer* renderer, int winW, int winH);

private:
    std::vector<Card> drawPile;
    std::vector<Card> hand;
    std::vector<Card> discardPile;
};
