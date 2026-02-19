#pragma once
#include "../Effect.h"
#include <algorithm>

class ShieldEffect : public Effect {
    int amount = 0;
public:
    ShieldEffect(int value) : amount(value) {}
    ShieldEffect() = default;

    void apply(CombatState& self, CombatState& target, int value) override {
        // if value is passed use that value else use internal mount
        int finalVal = (value != 0) ? value : amount;
        self.addShield(finalVal);
    }
};
