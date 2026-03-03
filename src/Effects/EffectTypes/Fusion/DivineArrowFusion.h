#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class DivineArrowFusion : public Effect {
    int amount = 0;
public:
    DivineArrowFusion(int value) : amount(value) {}
    DivineArrowFusion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        // Deal damage
        target.takeDamage(self.getModifiedDamage(finalVal));
        
        // Apply Bleed + Blessed (heals each turn!)
        target.applyStatus(std::make_unique<BleedStatus>(3, finalVal));
        self.applyStatus(std::make_unique<BlessedStatus>(2, finalVal));
        
        // If transformed, also apply Judged!
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<JudgedStatus>(2, finalVal));
        }
    }
};
