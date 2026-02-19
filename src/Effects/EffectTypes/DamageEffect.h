#pragma once
#include "../Effect.h"
#include <algorithm>

class DamageEffect : public Effect {
    int amount = 0;
public:
    DamageEffect(int value) : amount(value) {}
    DamageEffect() = default;

    void apply(CombatState& self, CombatState& target, int value) override {
        // if value is passed use that value else use internal mount
        int finalVal = (value != 0) ? value : amount;
        target.takeDamage(finalVal);
    }
};
