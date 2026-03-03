#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class UniversalSingularity : public Effect {
    int amount = 0;
public:
    UniversalSingularity(int value) : amount(value) {}
    UniversalSingularity() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        // Void Mark x10!
        target.applyStatus(std::make_unique<VoidMarkStatus>(5, 10));
        
        // True Void x10 for massive mana!
        self.applyStatus(std::make_unique<TrueVoidStatus>(5, 10));
        
        // Max corruption!
        self.corruption.current = self.corruption.max;
        
        // Overload!
        self.applyStatus(std::make_unique<OverloadStatus>(3, finalVal));
        
        // If transformed, become THE VOID!
        if (self.isTransformed) {
            // Deal damage equal to void mark intensity
            target.takeDamage(50);
            self.heal(30);
            self.applyStatus(std::make_unique<CorruptedVoidStatus>(2, 5));
        }
    }
};
