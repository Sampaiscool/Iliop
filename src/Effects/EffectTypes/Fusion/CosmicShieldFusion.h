#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class CosmicShieldFusion : public Effect {
    int amount = 0;
public:
    CosmicShieldFusion(int value) : amount(value) {}
    CosmicShieldFusion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        // Massive shield!
        self.addShield(finalVal * 3);
        
        // Gain True Void for extra mana!
        self.applyStatus(std::make_unique<TrueVoidStatus>(3, finalVal));
        
        // If transformed, become COSMIC! Extra shield + heal!
        if (self.isTransformed) {
            self.addShield(finalVal * 2);
            self.heal(finalVal * 2);
            self.applyStatus(std::make_unique<DefenceUpStatus>(2, finalVal));
        }
    }
};
