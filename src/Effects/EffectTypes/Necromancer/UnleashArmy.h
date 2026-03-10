#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class UnleashArmy : public Effect {
    int amount = 0;
public:
    UnleashArmy(int value) : amount(value) {}
    UnleashArmy() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.applyStatus(std::make_unique<ZombieArmyStatus>(1, finalVal));
        self.applyStatus(std::make_unique<SkeletonArmyStatus>(1, finalVal));

        target.applyStatus(std::make_unique<DeathMarkStatus>(3, finalVal));
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, 1));

        self.heal(finalVal * 3);
    }
};
