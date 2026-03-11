#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"

class PerfectCreation : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.addCardToHand("Machine Power", true);
    }
};
