#pragma once
#include "../Effect.h"
#include <algorithm>

class HealEffect : public Effect {
    int amount;

public:
    HealEffect(int heal) : amount(heal) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {


        // check raw heal
        int finalHeal = amount;
        if (isCorrupted) {
            finalHeal += static_cast<int>(amount * 0.5f);
        }

        // pipleline it up
        self.heal(finalHeal);
    }
};
