#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class AcidSplashEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));
        target.applyStatus(std::make_unique<DamageDownStatus>(2, 2));
        
        if (self.isTransformed) {
            target.applyStatus(std::make_unique<DefenceDownStatus>(2, 2));
            target.takeDamage(self.getModifiedDamage(value / 2));
        }
    }
};
