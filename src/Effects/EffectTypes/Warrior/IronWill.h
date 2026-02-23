#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class IronWill : public Effect {
    int amount = 0;
public:
    IronWill(int value) : amount(value) {}
    IronWill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        self.takeDamage(self.getModifiedDamage(value /2));
        self.addShield(value);
    }
};
