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
          CombatState&& stats,
          int threshold,
          int duration)
    : type(type),
      identity(identity),
      nameStr(nameStr),
      starterDeck(std::move(deck)),
      baseStats(std::move(stats))
    {
        baseStats.transformThreshold = threshold;
        baseStats.transformTime = 0;
        baseStats.transformGain = duration;
        baseStats.isTransformed = false;
    }
    int getTransformCorruption() const { return transformCorruption; }
    int getTransformTime() const { return transformTime; }
    void reduceTransformTime(int amount) { transformTime -= amount; }

private:
    int transformCorruption;
    int transformTime;
};
