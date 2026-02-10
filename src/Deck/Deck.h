#pragma once
#include <vector>
#include <list>
#include "../Card/Card.h"

class Deck {
public:
    Deck();
    void shuffle();
    void drawCard(int amount);
    void discardCard(const Card& card);
    void render(SDL_Renderer* renderer, int winW, int winH);

    std::vector<Card>& getHand();

private:
    std::vector<Card> drawPile;
    std::vector<Card> hand;
    std::vector<Card> discardPile;
};
