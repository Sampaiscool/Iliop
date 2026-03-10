#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class ArmySurge : public Effect {
    int amount = 0;
public:
    ArmySurge(int value) : amount(value) {}
    ArmySurge() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        if (self.isTransformed) {
            self.transformTime += 3;
            self.applyStatus(std::make_unique<ZombieArmyStatus>(1, finalVal * 2));
            self.applyStatus(std::make_unique<SkeletonArmyStatus>(1, finalVal * 2));
            self.heal(finalVal * 3);
        } else {
            self.transform(target);
            self.applyStatus(std::make_unique<ZombieArmyStatus>(1, finalVal));
            self.applyStatus(std::make_unique<SkeletonArmyStatus>(1, finalVal));
            self.applyStatus(std::make_unique<SoulFragmentStatus>(3, finalVal));
        }
    }
};
