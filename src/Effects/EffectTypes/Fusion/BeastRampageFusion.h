#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BeastRampageFusion : public Effect {
    int amount = 0;
public:
    BeastRampageFusion(int value) : amount(value) {}
    BeastRampageFusion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        // Big damage!
        target.takeDamage(self.getModifiedDamage(finalVal * 2));
        
        // Apply Defence Up + Damage Up!
        self.applyStatus(std::make_unique<DefenceUpStatus>(2, finalVal));
        self.applyStatus(std::make_unique<DamageUpStatus>(2, finalVal));
        
        // If transformed, RAMPAGE! Extra hit + bleed!
        if (self.isTransformed) {
            target.takeDamage(self.getModifiedDamage(finalVal));
            target.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
            // Also extend transformation!
            self.transformTime += 2;
        }
    }
};
