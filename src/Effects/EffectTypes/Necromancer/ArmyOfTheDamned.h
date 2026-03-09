#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class ArmyOfTheDamned : public Effect {
    int amount = 0;
public:
    ArmyOfTheDamned(int value) : amount(value) {}
    ArmyOfTheDamned() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.applyStatus(std::make_unique<ZombieArmyStatus>(0, finalVal * 2));
        self.applyStatus(std::make_unique<SkeletonArmyStatus>(0, finalVal * 2));
        self.applyStatus(std::make_unique<SoulFragmentStatus>(0, finalVal * 2));
        
        self.heal(finalVal * 4);
        
        if (finalVal >= 3) {
            target.applyStatus(std::make_unique<DeathMarkStatus>(3, finalVal));
        }
    }
};
