#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BloodFrenzyFusion : public Effect {
    int amount = 0;
public:
    BloodFrenzyFusion(int value) : amount(value) {}
    BloodFrenzyFusion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        // Damage + Apply Bleed!
        target.takeDamage(self.getModifiedDamage(finalVal));
        target.applyStatus(std::make_unique<BleedStatus>(3, finalVal));
        
        // Heal based on bleed damage!
        self.heal(finalVal);
        
        // If transformed or low HP, go into BLOOD FRENZY!
        if (self.isTransformed || self.hp.current < self.hp.max / 2) {
            // Apply Damage Up for next turn!
            self.applyStatus(std::make_unique<DamageUpStatus>(2, finalVal));
            // Extra hit!
            target.takeDamage(self.getModifiedDamage(finalVal / 2));
        }
    }
};
