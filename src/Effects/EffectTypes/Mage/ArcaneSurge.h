#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class ArcaneSurgeEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        for (int i = 0; i < 3; ++i) {
            target.takeDamage(self.getModifiedDamage(value));
        }
        self.gainCorruption(1);
        
        if (self.isTransformed) {
            target.takeDamage(self.getModifiedDamage(value));
            target.takeDamage(self.getModifiedDamage(value));
            self.gainCorruption(2);
        }
    }
};
