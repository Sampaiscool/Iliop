#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class AstralShift : public Effect {
    int amount = 0;
public:
    AstralShift(int value) : amount(value) {}
    AstralShift() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        self.addShield(value + self.corruption.current);

        if (self.isTransformed) {
            // if you are transformed, gain more transformtime
            self.transformTime += 2;
        } else {
            // if you are nor transformed remove all corruption
            self.corruption.current = 0;
        }
    }
};
