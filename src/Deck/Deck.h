#pragma once
#include <vector>
#include <memory>
#include "../Card/Card.h"
#include "../Effects/Effect.h"
#include "../Other/Particle.h"
#include <SFML/Graphics.hpp>

class Deck {
public:
    Deck() = default;
    Deck(std::vector<Card> startingCards);

    void setDeck(std::vector<Card> cards);
    void shuffle();
    void drawCard(int amount);
    void addCardToPermanentCollection(Card card);
    void discardCard(Card& card);
    void discardHand();
    void render(sf::RenderWindow& window, int winW, int winH, const sf::Font& font, const CombatState& player, ParticleSystem& particles);

    std::vector<Card>& getHand();
    std::vector<Card>& getPermanentCollection();
    void addCardToDrawPile(Card card);
    void removeCardFromDrawPile(const std::string& cardName);
    void syncAugmentations();

private:
    std::vector<Card> drawPile;
    std::vector<Card> hand;
    std::vector<Card> discardPile;
    std::vector<Card> permanentCollection;
};

