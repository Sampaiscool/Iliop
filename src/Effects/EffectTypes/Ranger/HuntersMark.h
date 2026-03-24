#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class HuntersMarkEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));
        target.applyStatus(std::make_unique<DamageUpStatus>(2, 3));
        target.applyStatus(std::make_unique<BleedStatus>(1, 2));
        
        if (self.isTransformed) {
            target.applyStatus(std::make_unique<DamageDownStatus>(2, 2));
            self.heal(value / 2);
        }
    }
};
