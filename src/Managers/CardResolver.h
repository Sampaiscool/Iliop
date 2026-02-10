#pragma once
#include <SDL2/SDL.h>
#include "../Card/Card.h"
#include "../Other/CombatState.h"

class CardResolver {
public:
    static void play(const Card& card, CombatState& player, CombatState& enemy);
};
