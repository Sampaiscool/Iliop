#pragma once
#include "../Effect.h"
#include <algorithm>

class HealEffect : public Effect {
    int amount;

public:
    HealEffect(int heal) : amount(heal) {}

    void apply(CombatState& self, CombatState& target) override {
        self.hp.current += amount;
        if (self.hp.current > self.hp.max)
            self.hp.current = self.hp.max;
    }
};
