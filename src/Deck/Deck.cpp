#include "Deck.h"
#include <algorithm>
#include <random>
#include <iostream>

int deckCardAmount = 40;

Deck::Deck() {
    // build deck
    for (int i = 0; i < deckCardAmount; ++i) {
        Card card;
        card.type = static_cast<CardType>(i % 3);
        switch (card.type) {
            case CardType::Damage:
                card.name = "Magic Blast";
                card.effect = CardEffect::DealDamage;
                card.cost = 1;
                break;
            case CardType::Heal:
                card.name = "Healing Winds";
                card.effect = CardEffect::Heal;
                card.cost = 1;
                break;
            case CardType::Shield:
                card.name = "Protecting Barrier";
                card.effect = CardEffect::GainShield;
                card.cost = 1;
                break;
        }
        card.value = 5 + (i % 5);
        drawPile.push_back(card);
    }
}

std::vector<Card>& Deck::getHand() {
    return hand;
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(drawPile.begin(), drawPile.end(), g);
}

void Deck::drawCard(int amount) {
    for (int i = 0; i < amount; ++i) {
        if (drawPile.empty()) {
            // reshuffle discard pile if draw pile is empty
            drawPile = std::move(discardPile);
            discardPile.clear();
            shuffle();
            if (drawPile.empty()) break;
        }
        hand.push_back(drawPile.back());
        drawPile.pop_back();
    }
}

void Deck::discardCard(const Card& card) {
    // remove card from hand
    auto it = std::find_if(hand.begin(), hand.end(),
        [&](const Card& c) { return &c == &card; });
    if (it != hand.end()) {
        discardPile.push_back(*it);
        hand.erase(it);
    }
}

void Deck::render(sf::RenderWindow& window, int winW, int winH, const sf::Font& font) {
    int cardW = winW / 10;
    int cardH = winH / 6;
    int handAreaWidth = static_cast<int>(winW * 0.6f);
    int startX = 20;
    int spacing = 0;
    if (hand.size() > 1)
        spacing = (handAreaWidth - cardW) / (hand.size() - 1);
    else
        spacing = 0;
    
    int handY = winH - cardH - 40;

    for (size_t i = 0; i < hand.size(); ++i) {
        Card& card = hand[i];
        card.x = startX + spacing * i;
        card.y = handY;
        card.w = cardW;
        card.h = cardH;

        card.draw(window, font);
    }
}


