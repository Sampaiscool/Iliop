#pragma once
#include "../Effect.h"
#include <algorithm>

class ShieldEffect : public Effect {
    int amount;

public:
    ShieldEffect(int shield) : amount(shield) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {

        int bonus = 0;

        if (isCorrupted) {
            bonus = static_cast<int>(amount * 0.5f);
        }

        self.shield.current += amount + bonus;

        if (self.shield.current > self.shield.max)
            self.shield.current = self.shield.max;
    }
};
