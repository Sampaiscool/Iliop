#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BoneSplinter : public Effect {
    int amount = 0;
public:
    BoneSplinter(int value) : amount(value) {}
    BoneSplinter() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int skeletonBonus = self.getSkeletonArmyIntensity();
        int totalDamage = self.getModifiedDamage(finalVal + skeletonBonus);
        
        target.takeDamage(totalDamage);
        
        if (self.isTransformed) {
            target.applyStatus(std::make_unique<DefenceDownStatus>(2, std::max(skeletonBonus / 2, 1)));
            self.applyStatus(std::make_unique<SoulFragmentStatus>(3, 1));
        }
    }
};
