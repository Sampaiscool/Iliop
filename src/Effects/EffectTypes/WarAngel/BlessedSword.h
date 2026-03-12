#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BlessedSword : public Effect {
    int amount = 0;
public:
    BlessedSword(int value) : amount(value) {}
    BlessedSword() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        int slashAmount = (self.isTransformed) ? (3) : 1;

        for (int i = 0; i < slashAmount; i++) {
            int damageDone = target.takeDamage(self.getModifiedDamage(finalVal));
            target.applyStatus(std::make_unique<BlessedStatus>(2, damageDone));
        }
    }
};
