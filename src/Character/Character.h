#pragma once
#include <vector>
#include "../Card/Card.h"
#include "../Deck/Deck.h"
#include "../Other/CombatState.h"

enum class Class {
    Mage,
    Warrior,
    Cleric
};

enum class CharacterName {
    Hiroshi,
    Phlox,
    MightyFire
};

class Character {
public:
    Class type;
    CharacterName identity;
    std::string nameStr;
    std::vector<Card> starterDeck;
    CombatState baseStats;

    Character(Class type,
              CharacterName identity,
              std::string nameStr,
              std::vector<Card> deck,
              CombatState stats,
              int threshold)
        : type(type),
          identity(identity),
          nameStr(nameStr),
          starterDeck(std::move(deck)),
          baseStats(stats),
          transformCorruption(threshold) {
              baseStats.transformThreshold = threshold;
          }

    int getTransformCorruption() const { return transformCorruption; }

private:
    int transformCorruption;
};
