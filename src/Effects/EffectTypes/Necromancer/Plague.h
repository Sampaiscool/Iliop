#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class PlagueEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));
        target.applyStatus(std::make_unique<BleedStatus>(3, 2));
        target.applyStatus(std::make_unique<DamageDownStatus>(2, 2));
        target.applyStatus(std::make_unique<StunStatus>(1, 1));
        
        if (self.isTransformed) {
            target.applyStatus(std::make_unique<BleedStatus>(3, 2));
            target.applyStatus(std::make_unique<DefenceDownStatus>(2, 2));
            self.heal(3);
        }
    }
};
