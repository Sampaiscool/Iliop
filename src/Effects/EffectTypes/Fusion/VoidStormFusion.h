#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class VoidStormFusion : public Effect {
    int amount = 0;
public:
    VoidStormFusion(int value) : amount(value) {}
    VoidStormFusion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        // Massive damage!
        target.takeDamage(self.getModifiedDamage(finalVal * 2));
        
        // Apply Void Mark (builds up to huge effects!)
        target.applyStatus(std::make_unique<VoidMarkStatus>(3, 3));
        
        // Gain corruption + if transformed, gain True Void!
        self.corruption.current = std::min(self.corruption.current + 2, self.corruption.max);
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<TrueVoidStatus>(3, 3));
            // Also apply overload for extra damage
            self.applyStatus(std::make_unique<OverloadStatus>(2, finalVal));
        }
    }
};
