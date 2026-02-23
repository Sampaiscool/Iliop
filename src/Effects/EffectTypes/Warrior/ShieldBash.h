#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class ShieldBash : public Effect {
    int amount = 0;
public:
    ShieldBash(int value) : amount(value) {}
    ShieldBash() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        if (self.shield.current >= (self.shield.max)) {
            int damage = self.getModifiedDamage(value + (self.shield.current / 2));
            target.takeDamage(damage);
            target.applyStatus(std::make_unique<StunStatus>(1, 1));
            self.shield.current = 0;
        }
    }
};
