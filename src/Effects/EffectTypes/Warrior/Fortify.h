#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Fortify : public Effect {
    int amount = 0;
public:
    Fortify(int value) : amount(value) {}
    Fortify() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        self.addShield(self.getModifiedDamage(finalVal * 2));
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<DefenceUpStatus>(2, finalVal));
            self.addShield(self.shield.max / 2);
        }
        
        if (self.shield.current >= self.shield.max) {
            self.applyStatus(std::make_unique<DamageUpStatus>(2, finalVal));
        }
    }
};
