#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BoneLegion : public Effect {
    int amount = 0;
public:
    BoneLegion(int value) : amount(value) {}
    BoneLegion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        self.applyStatus(std::make_unique<SkeletonArmyStatus>(99, finalVal));
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<SkeletonArmyStatus>(99, finalVal));
            self.addShield(finalVal * 2);
        }
    }
};
