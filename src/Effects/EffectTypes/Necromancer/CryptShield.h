#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class CryptShield : public Effect {
    int amount = 0;
public:
    CryptShield(int value) : amount(value) {}
    CryptShield() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int skeletonBonus = self.getSkeletonArmyIntensity();
        int shieldGain = finalVal + skeletonBonus;
        
        self.addShield(shieldGain);
        
        if (self.isTransformed) {
            self.addShield(skeletonBonus);
            self.applyStatus(std::make_unique<DefenceUpStatus>(2, skeletonBonus / 2 + 1));
        }
        
        if (skeletonBonus >= 3) {
            self.applyStatus(std::make_unique<SkeletonArmyStatus>(99, 1));
        }
    }
};
