#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Reaction : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));

        self.heal(5);
        self.addShield(5);

        // Remove 2 random metal statuses
        self.removeRandomMetalStatus();
        self.removeRandomMetalStatus();
    }
};
