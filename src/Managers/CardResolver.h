#pragma once
#include "../Card/Card.h"
#include "../Other/CombatState.h"

class CardResolver {
public:
    static bool play(const Card& card, CombatState& player, CombatState& enemy);
};
