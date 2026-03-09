#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class EnemyPoisonStrike : public Effect {
    int amount = 0;
public:
    EnemyPoisonStrike(int value) : amount(value) {}
    EnemyPoisonStrike() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int dmg = (value != 0) ? value : amount;
        target.takeDamage(dmg);
        target.applyStatus(std::make_unique<BleedStatus>(3, 2));
    }
};
