#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class VoidProwess : public Effect {
    int amount = 0;
public:
    VoidProwess(int value) : amount(value) {}
    VoidProwess() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        if (value != 0) {
            self.applyStatus(std::make_unique<TrueVoidStatus>(2, value));
        } else {
            self.applyStatus(std::make_unique<TrueVoidStatus>(2, amount));
        }
    }
};
