#pragma once
#include "../Effect.h"
#include <algorithm>

class HealEffect : public Effect {
    int amount = 0;
public:
    HealEffect(int value) : amount(value) {}
    HealEffect() = default;

    void apply(CombatState& self, CombatState& target, int value) override {
        // if value is passed use that value else use internal mount
        int finalVal = (value != 0) ? value : amount;
        self.heal(finalVal);
    }
};
