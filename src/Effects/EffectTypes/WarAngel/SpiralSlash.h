#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class SpiralSlash : public Effect {
    int amount = 0;
public:
    SpiralSlash(int value) : amount(value) {}
    SpiralSlash() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal));

        for (int i = 0; i < finalVal; i++) {
            self.addCardToHand("Spirit Slash", true);
        }

        if (self.corruption.current < 2) {
            self.applyStatus(std::make_unique<FlightStatus>(3, finalVal / 2));
        }

        if (self.isTransformed) {
            self.applyStatus(std::make_unique<TrueVoidStatus>(3, finalVal / 2));
        }
    }
};
