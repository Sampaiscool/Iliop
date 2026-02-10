#include "Deck.h"
#include <algorithm>
#include <random>

static const int deckCardAmount = 40;

Deck::Deck() {
    for (int i = 0; i < deckCardAmount; ++i) {
        Card card;
        card.type = static_cast<CardType>(i % 3);
        switch (card.type) {
            case CardType::Damage:
                card.name = "Magic Blast";
                card.effect = CardEffect::DealDamage;
                break;
            case CardType::Heal:
                card.name = "Healing Winds";
                card.effect = CardEffect::Heal;
                break;
            case CardType::Shield:
                card.name = "Protecting Barrier";
                card.effect = CardEffect::GainShield;
                break;
        }
        card.value = 5 + (i % 5);
        drawPile.push_back(card);
    }
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(drawPile.begin(), drawPile.end(), g);
}

void Deck::drawCard(int amount) {
    for (int i = 0; i < amount && !drawPile.empty(); ++i) {
        hand.push_back(drawPile.back());
        drawPile.pop_back();
    }
}

void Deck::discardCard(const Card& card) {
    auto it = std::find_if(hand.begin(), hand.end(),
        [&](const Card& c){ return &c == &card; });
    if (it != hand.end()) {
        discardPile.push_back(*it);
        hand.erase(it);
    }
}

std::vector<Card>& Deck::getHand() {
    return hand;
}


void Deck::render(sf::RenderWindow& window, int winW, int winH, const sf::Font& font) {
    int cardW = winW / 10;
    int cardH = winH / 6;

    int handAreaW = winW * 0.6f;
    int handStartX = 40;
    int handY = winH - cardH - 40;

    int spacing = hand.size() > 0 ? handAreaW / hand.size() : 0;

    for (size_t i = 0; i < hand.size(); ++i) {
        Card& card = hand[i];
        card.x = handStartX + spacing * i;
        card.y = handY;
        card.w = cardW;
        card.h = cardH;

        card.draw(window, font);
    }
}

