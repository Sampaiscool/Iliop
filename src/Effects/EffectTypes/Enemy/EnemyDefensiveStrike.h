#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class EnemyDefensiveStrike : public Effect {
    int amount = 0;
public:
    EnemyDefensiveStrike(int value) : amount(value) {}
    EnemyDefensiveStrike() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int val = (value != 0) ? value : amount;
        self.shield.current += val;
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, 2));
    }
};
