#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class HolyArrows : public Effect {
    int amount = 0;
public:
    HolyArrows(int value) : amount(value) {}
    HolyArrows() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.applyStatus(std::make_unique<LockedStatus>(2, finalVal));

        if (self.corruption.current = self.corruption.max) {
            self.applyStatus(std::make_unique<DamageUpStatus>(2, finalVal));
        } else if (self.corruption.current > (self.corruption.max / 2)) {
            self.gainCorruption(self.corruption.current);
        }

        if (self.isTransformed) {
            self.applyStatus(std::make_unique<BlessedStatus>(2, finalVal));
        }
   }
};
