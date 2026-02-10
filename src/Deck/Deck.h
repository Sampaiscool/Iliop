#pragma once
#include <vector>
#include "../Card/Card.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

class Deck {
public:
    Deck();
    void shuffle();
    void drawCard(int amount);
    void discardCard(const Card& card);
    void render(sf::RenderWindow& window, int winW, int winH, const sf::Font& font);

    std::vector<Card>& getHand();

private:
    std::vector<Card> drawPile;
    std::vector<Card> hand;
    std::vector<Card> discardPile;
};

