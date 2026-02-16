#pragma once
#include "../Effect.h"
#include <algorithm>

class HealEffect : public Effect {
    int amount;

public:
    HealEffect(int heal) : amount(heal) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {

        int bonus = 0;

        if (isCorrupted) {
            bonus = static_cast<int>(amount * 0.5f);
        }

        self.hp.current += amount + bonus;

        if (self.hp.current > self.hp.max)
            self.hp.current = self.hp.max;
    }
};
