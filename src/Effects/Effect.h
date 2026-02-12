#pragma once
#include "../Other/CombatState.h"

class Effect {
public:
    virtual ~Effect() = default;
    virtual void apply(CombatState& self, CombatState& target) = 0;
};
