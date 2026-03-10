#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Toss : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.removeRandomMetalCardFromHand();
        target.takeDamage(self.getModifiedDamage(value));
    }
};
