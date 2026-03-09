#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class GraveCall : public Effect {
    int amount = 0;
public:
    GraveCall(int value) : amount(value) {}
    GraveCall() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
                
        self.applyStatus(std::make_unique<ZombieArmyStatus>(0, finalVal));
        self.applyStatus(std::make_unique<SkeletonArmyStatus>(0, finalVal));
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<ZombieArmyStatus>(0, finalVal));
            self.applyStatus(std::make_unique<SkeletonArmyStatus>(0, finalVal));
            self.heal(finalVal * 2);
        }
    }
};
