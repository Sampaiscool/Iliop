#pragma once
#include <vector>
#include <memory>
#include "../Card/Card.h"
#include "../Effects/Effect.h"
#include <SFML/Graphics.hpp>

class Deck {
public:
    Deck();
    void shuffle();
    void drawCard(int amount);
    void discardCard(Card& card);
    void discardHand();
    void render(sf::RenderWindow& window, int winW, int winH, const sf::Font& font);

    std::vector<Card>& getHand();

private:
    std::vector<Card> drawPile;
    std::vector<Card> hand;
    std::vector<Card> discardPile;
};

