#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BloodRampageEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));
        self.takeDamage(value / 2);
        
        if (self.isTransformed) {
            target.takeDamage(self.getModifiedDamage(value));
            self.heal(value);
        }
    }
};
