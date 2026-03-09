#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class EnemyWeakening : public Effect {
    int amount = 0;
public:
    EnemyWeakening(int value) : amount(value) {}
    EnemyWeakening() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int dmg = (value != 0) ? value : amount;
        target.takeDamage(dmg);
        target.applyStatus(std::make_unique<DamageDownStatus>(2, 2));
    }
};
