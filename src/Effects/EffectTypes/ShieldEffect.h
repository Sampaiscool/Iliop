#pragma once
#include "../Effect.h"
#include <algorithm>

class ShieldEffect : public Effect {
    int amount;

public:
    ShieldEffect(int shield) : amount(shield) {}

    void apply(CombatState& self, CombatState& target) override {
        self.shield.current += amount;
        if (self.shield.current > self.shield.max)
            self.shield.current = self.shield.max;
    }
};
