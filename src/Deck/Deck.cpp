#include "Deck.h"
#include <algorithm>
#include <random>

int deckCardAmount = 40;
int initialHandSize = 4;

Deck::Deck() {
    // build deck
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

void Deck::drawInitialHand() {
    for (int i = 0; i < initialHandSize; ++i) {
        hand.push_back(drawPile.back());
        drawPile.pop_back();
    }
}

void Deck::render(SDL_Renderer* renderer, int winW, int winH) {
    int cardW = winW / 10;
    int cardH = winH / 6;
    int spacing = winW / (hand.size() + 1);
    int handY = winH - cardH - 40;

    for (size_t i = 0; i < hand.size(); ++i) {
        hand[i].x = spacing * (i + 1) - cardW / 2;
        hand[i].y = handY;
        hand[i].w = cardW;
        hand[i].h = cardH;

        hand[i].draw(renderer);
    }
}
