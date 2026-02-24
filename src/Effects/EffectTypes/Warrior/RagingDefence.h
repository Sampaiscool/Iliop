#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class RagingDefence : public Effect {
    int amount = 0;
public:
    RagingDefence(int value) : amount(value) {}
    RagingDefence() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        if (self.shield.current > (self.shield.max / 2)) {
            self.takeDamage(amount);
            target.takeDamage(self.getModifiedDamage(amount * 2));
        } else {
            self.addShield(amount);
        }
    }
};
