#pragma once
#include "../Effect.h"
#include <algorithm>

class HealEffect : public Effect {
    int amount;

public:
    HealEffect(int heal) : amount(heal) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {

        if (isCorrupted) {
          amount = static_cast<int>(amount * 1.5f);
        }

        self.hp.current += amount;
        if (self.hp.current > self.hp.max)
            self.hp.current = self.hp.max;
    }
};
