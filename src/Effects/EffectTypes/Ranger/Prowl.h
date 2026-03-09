#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Prowl : public Effect {
    int amount = 0;
public:
    Prowl(int value) : amount(value) {}
    Prowl() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        if (self.hp.current >= self.hp.max) {
            target.takeDamage(self.getModifiedDamage(finalVal));
            target.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
            self.applyStatus(std::make_unique<DamageUpStatus>(1, finalVal));
        } else {
            target.takeDamage(self.getModifiedDamage(finalVal));
            self.applyStatus(std::make_unique<LockedStatus>(1, finalVal / 2));
            self.heal(finalVal);
        }
    }
};
