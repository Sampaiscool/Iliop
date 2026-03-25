#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class CatalystBomb : public Effect {
    int amount = 0;
public:
    CatalystBomb(int value) : amount(value) {}
    CatalystBomb() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        // i am not real 
    }
};
