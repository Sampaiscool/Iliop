#pragma once
#include "../Card/Card.h"
#include "../Deck/Deck.h"
#include "../Other/Status.h"
#include "../Other/CombatState.h"

class CardResolver {
public:
    static bool play(const Card& card, CombatState& player, CombatState& enemy, Deck& deck);
};
