#pragma once
#include "../Other/CombatState.h"

class Effect {
public:
    virtual ~Effect() = default;
    virtual void apply(CombatState& self, CombatState& target, int value) = 0;
};
