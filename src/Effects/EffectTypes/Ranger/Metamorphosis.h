#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Metamorphosis : public Effect {
    int amount = 0;
public:
    Metamorphosis(int value) : amount(value) {}
    Metamorphosis() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        if (self.isTransformed) {
            self.transformTime += 3;
            self.heal(finalVal * 2);
        } else {
            self.transform(target);
            self.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
        }
    }
};
