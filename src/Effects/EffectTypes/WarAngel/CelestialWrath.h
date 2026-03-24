#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class CelestialWrathEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalValue = value;
        if (self.isTransformed) {
            finalValue = value * 2;
        }
        
        target.takeDamage(self.getModifiedDamage(finalValue));
        self.applyStatus(std::make_unique<DamageUpStatus>(2, 2));
        
        if (self.isTransformed) {
            self.addShield(finalValue / 2);
            self.applyStatus(std::make_unique<BlessedStatus>(2, 2));
        }
    }
};
