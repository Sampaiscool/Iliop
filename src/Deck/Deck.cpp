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
    // first populate permanent collection with starter deck (with any augmentations)
    permanentCollection = std::move(cards);
    // rebuild draw pile from permanent collection (preserving augmentations)
    drawPile.clear();
    for (auto& permCard : permanentCollection) {
        Card newCard = CardFactory::create(permCard.name);
        // copy augmentations
        newCard.bonusValue = permCard.bonusValue;
        newCard.costReduction = permCard.costReduction;
        newCard.drawOnUse = permCard.drawOnUse;
        newCard.replayCount = permCard.replayCount;
        newCard.freeOnceCount = permCard.freeOnceCount;
        newCard.applyDamageUp = permCard.applyDamageUp;
        newCard.applyDefenceUp = permCard.applyDefenceUp;
        newCard.applyRegen = permCard.applyRegen;
        newCard.applyBleed = permCard.applyBleed;
        newCard.healOnUse = permCard.healOnUse;
        newCard.shieldOnUse = permCard.shieldOnUse;
        drawPile.push_back(std::move(newCard));
    }
    discardPile.clear();
    hand.clear();
}

std::vector<Card>& Deck::getHand() {
    return hand;
}

std::vector<Card>& Deck::getPermanentCollection() {
    return permanentCollection;
}

void Deck::addCardToDrawPile(Card card) {
    // look up imprints from permanent collection
    for (const auto& permCard : permanentCollection) {
        if (permCard.name == card.name) {
            card.bonusValue = permCard.bonusValue;
            card.costReduction = permCard.costReduction;
            card.drawOnUse = permCard.drawOnUse;
            card.replayCount = permCard.replayCount;
            card.freeOnceCount = permCard.freeOnceCount;
            card.applyDamageUp = permCard.applyDamageUp;
            card.applyDefenceUp = permCard.applyDefenceUp;
            card.applyRegen = permCard.applyRegen;
            card.applyBleed = permCard.applyBleed;
            card.healOnUse = permCard.healOnUse;
            card.shieldOnUse = permCard.shieldOnUse;
            break;
        }
    }
    drawPile.push_back(std::move(card));
}

void Deck::removeCardFromDeck(const std::string& cardName) {
    // remove ONE copy from each pile
    for (auto it = drawPile.begin(); it != drawPile.end(); ++it) {
        if (it->name == cardName) {
            drawPile.erase(it);
            break;
        }
    }
    for (auto it = discardPile.begin(); it != discardPile.end(); ++it) {
        if (it->name == cardName) {
            discardPile.erase(it);
            break;
        }
    }
    for (auto it = hand.begin(); it != hand.end(); ++it) {
        if (it->name == cardName) {
            hand.erase(it);
            break;
        }
    }
}

void Deck::rebuildFromPermanent() {
    // rebuild draw pile from permanent collection
    drawPile.clear();
    for (auto& permCard : permanentCollection) {
        Card newCard = CardFactory::create(permCard.name);
        newCard.bonusValue = permCard.bonusValue;
        newCard.costReduction = permCard.costReduction;
        newCard.drawOnUse = permCard.drawOnUse;
        newCard.replayCount = permCard.replayCount;
        newCard.freeOnceCount = permCard.freeOnceCount;
        newCard.applyDamageUp = permCard.applyDamageUp;
        newCard.applyDefenceUp = permCard.applyDefenceUp;
        newCard.applyRegen = permCard.applyRegen;
        newCard.applyBleed = permCard.applyBleed;
        newCard.healOnUse = permCard.healOnUse;
        newCard.shieldOnUse = permCard.shieldOnUse;
        drawPile.push_back(std::move(newCard));
    }
    // clear other piles since we rebuild from scratch
    discardPile.clear();
    hand.clear();
    // shuffle the new draw pile
    shuffle();
    // draw new cards bc we DESTROYED the hand
    drawCard(4);
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
    // add to permanent collection
    const std::string cardName = card.name;
    permanentCollection.push_back(std::move(card));
    
    // also add a copy to draw pile with the same imprints
    for (const auto& permCard : permanentCollection) {
        if (permCard.name == cardName) {
            Card newCard = CardFactory::create(permCard.name);
            newCard.bonusValue = permCard.bonusValue;
            newCard.costReduction = permCard.costReduction;
            newCard.drawOnUse = permCard.drawOnUse;
            newCard.replayCount = permCard.replayCount;
            newCard.freeOnceCount = permCard.freeOnceCount;
            newCard.applyDamageUp = permCard.applyDamageUp;
            newCard.applyDefenceUp = permCard.applyDefenceUp;
            newCard.applyRegen = permCard.applyRegen;
            newCard.applyBleed = permCard.applyBleed;
            newCard.healOnUse = permCard.healOnUse;
            newCard.shieldOnUse = permCard.shieldOnUse;
            drawPile.push_back(std::move(newCard));
            break;
        }
    }
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
