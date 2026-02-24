#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class AegisStrike : public Effect {
    int amount = 0;
public:
    AegisStrike(int value) : amount(value) {}
    AegisStrike() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int damage = self.getModifiedDamage(value);
        self.takeDamage(damage);
        target.takeDamage(damage);

        if (self.isTransformed) {
            self.addShield(value);
            self.takeDamage(damage);
            target.takeDamage(damage);
        }
        else {
            self.transform(target);
        }
    }
};
