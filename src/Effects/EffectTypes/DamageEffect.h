#pragma once
#include "../Effect.h"
#include <algorithm>

class DamageEffect : public Effect {
    int amount;

public:
    DamageEffect(int dmg) : amount(dmg) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {

        // check raw damage
        int finalDamage = amount;
        if (isCorrupted) {
            finalDamage += static_cast<int>(amount * 0.5f);
        }

        // send to pipline ples
        target.takeDamage(finalDamage);
    }
};
