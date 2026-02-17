#pragma once
#include "../Effect.h"
#include <algorithm>

class ShieldEffect : public Effect {
    int amount;

public:
    ShieldEffect(int shield) : amount(shield) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {

        // raw shield
        int finalShield = amount;
        if (isCorrupted) {
            finalShield += static_cast<int>(amount * 0.5f);
        }

        // send to pipline
        self.addShield(finalShield);
    }
};
