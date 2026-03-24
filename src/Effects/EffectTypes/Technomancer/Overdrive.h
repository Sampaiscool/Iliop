#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class OverdriveEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));
        self.applyStatus(std::make_unique<DamageUpStatus>(2, 2));
        self.gainCorruption(1);
        
        if (self.isTransformed) {
            target.takeDamage(self.getModifiedDamage(value));
            target.applyStatus(std::make_unique<DamageDownStatus>(2, 2));
            self.applyStatus(std::make_unique<DamageUpStatus>(2, 2));
        }
    }
};
