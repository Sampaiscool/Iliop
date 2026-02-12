#pragma once
#include <vector>
#include "../Card/Card.h"
#include "../Deck/Deck.h"
#include "../Other/CombatState.h"

enum class CharacterClass {
    Mage,
    Warrior,
    Cleric
};

class Character {
public:
    CharacterClass type;
    std::vector<Card> starterDeck;
    CombatState baseStats;

    Character(CharacterClass type,
              std::vector<Card> deck,
              CombatState stats,
              int threshold)
        : type(type),
          starterDeck(std::move(deck)),
          baseStats(stats),
          transformCorruption(threshold) {
              baseStats.transformThreshold = threshold;
          }

    int getTransformCorruption() const { return transformCorruption; }

private:
    int transformCorruption;
};
