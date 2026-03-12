#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class SpiritSlash : public Effect {
    int amount = 0;
public:
    SpiritSlash(int value) : amount(value) {}
    SpiritSlash() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.gainCorruption(finalVal);

        target.takeDamage(self.getModifiedDamage(finalVal));

        if (self.corruption.current == self.corruption.max) {
            target.applyStatus(std::make_unique<DamageDownStatus>(2, finalVal));
        }
   }
};
