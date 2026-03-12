#include "Deck.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include "../Managers/CardFactory.h"
#include <algorithm>
#include <random>

int deckCardAmount = 40;

Deck::Deck(std::vector<Card> startingCards) {
    drawPile = std::move(startingCards);
}

void Deck::setDeck(std::vector<Card> cards) {
    drawPile = std::move(cards);
    discardPile.clear();
    hand.clear();
    // populate permanent collection by creating copies of each card
    permanentCollection.clear();
    for (const auto& card : drawPile) {
        // create a new card using CardFactory to get a fresh copy
        Card copy = CardFactory::create(card.name);
        permanentCollection.push_back(std::move(copy));
    }
}

std::vector<Card>& Deck::getHand() {
    return hand;
}

std::vector<Card>& Deck::getPermanentCollection() {
    return permanentCollection;
}

void Deck::addCardToDrawPile(Card card) {
    drawPile.push_back(std::move(card));
}

void Deck::removeCardFromDrawPile(const std::string& cardName) {
    for (auto it = drawPile.begin(); it != drawPile.end(); ++it) {
        if (it->name == cardName) {
            drawPile.erase(it);
            return;
        }
    }
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
        hand.push_back(std::move(drawPile.back()));
        drawPile.pop_back();
    }
}

void Deck::discardCard(Card& card) {
    auto it = std::find_if(hand.begin(), hand.end(),
        [&](const Card& c) { return &c == &card; });

    if (it != hand.end()) {
        discardPile.push_back(std::move(*it));
        hand.erase(it);
    }
}

// void Deck::addCard(Card card) {
//     discardPile.push_back(std::move(card));
// } :)

void Deck::addCardToPermanentCollection(Card card) {
    permanentCollection.push_back(std::move(card));
}

void Deck::discardHand()
{
    for (auto& card : hand) {
        // dont add temp cards
        if (!card.isTemporary) {
            discardPile.push_back(std::move(card));
        }
    }

    hand.clear();
}

void Deck::render(sf::RenderWindow& window, int winW, int winH, const sf::Font& font, const CombatState& player, ParticleSystem& particles) {
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

    bool isCorrupted = (player.corruption.current >= player.transformThreshold);

    for (size_t i = 0; i < hand.size(); ++i) {
        Card& card = hand[i];
        card.x = startX + spacing * i;
        card.y = handY;
        card.w = cardW;
        card.h = cardH;

        card.draw(window, font, isCorrupted, player);

        if (isCorrupted) {
            // pick a random spot
            float spawnX = card.x + (rand() % card.w);
            float spawnY = card.y; 
            // emit particles per frame
            particles.emit({spawnX, spawnY}, 1);
        }
    }
}

void Deck::syncAugmentations() {
    // sync augmentations from permanent collection to draw pile and discard pile
    for (auto& permCard : permanentCollection) {
        for (auto& drawCard : drawPile) {
            if (drawCard.name == permCard.name) {
                drawCard.bonusValue = permCard.bonusValue;
                drawCard.costReduction = permCard.costReduction;
                drawCard.drawOnUse = permCard.drawOnUse;
                drawCard.replay = permCard.replay;
                drawCard.freeOnce = permCard.freeOnce;
            }
        }
        for (auto& discCard : discardPile) {
            if (discCard.name == permCard.name) {
                discCard.bonusValue = permCard.bonusValue;
                discCard.costReduction = permCard.costReduction;
                discCard.drawOnUse = permCard.drawOnUse;
                discCard.replay = permCard.replay;
                discCard.freeOnce = permCard.freeOnce;
            }
        }
    }
}
