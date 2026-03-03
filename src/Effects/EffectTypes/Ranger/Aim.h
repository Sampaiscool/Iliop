#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Aim : public Effect {
    int amount = 0;
public:
    Aim(int value) : amount(value) {}
    Aim() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        if (!self.isTransformed) {
            self.applyStatus(std::make_unique<LockedStatus>(3, value));
            self.heal(value);
        } else {
            self.applyStatus(std::make_unique<RagingBearStatus>(3, value));
            target.applyStatus(std::make_unique<BleedStatus>(2, value));
        }
    }
};
