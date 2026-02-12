#pragma once
#include "../Effect.h"
#include <algorithm>

class ShieldEffect : public Effect {
    int amount;

public:
    ShieldEffect(int shield) : amount(shield) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {

        if (isCorrupted) {
            amount = static_cast<int>(amount * 1.5f);
        }

        self.shield.current += amount;
        if (self.shield.current > self.shield.max)
            self.shield.current = self.shield.max;
    }
};
