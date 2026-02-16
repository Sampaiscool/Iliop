#pragma once
#include "../Effect.h"
#include <algorithm>

class DamageEffect : public Effect {
    int amount;

public:
    DamageEffect(int dmg) : amount(dmg) {}

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {

        int bonus = 0;

        if (isCorrupted) {
            bonus = static_cast<int>(amount * 0.5f);
        }

        int damage = amount + bonus;

        if (target.shield.current > 0) {
            int absorbed = std::min(target.shield.current, damage);
            target.shield.current -= absorbed;
            damage -= absorbed;
        }

        target.hp.current -= damage;
    }
};
