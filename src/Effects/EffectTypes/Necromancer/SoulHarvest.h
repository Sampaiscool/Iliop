#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class SoulHarvest : public Effect {
    int amount = 0;
public:
    SoulHarvest(int value) : amount(value) {}
    SoulHarvest() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        self.applyStatus(std::make_unique<SoulFragmentStatus>(5, finalVal));
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<SoulFragmentStatus>(0, finalVal * 2));
            self.heal(finalVal * 2);
        }
        
        if (self.getSoulFragmentCount() >= 5) {
            self.applyStatus(std::make_unique<ZombieArmyStatus>(0, 1));
            self.applyStatus(std::make_unique<SkeletonArmyStatus>(0, 1));
        }
    }
};
