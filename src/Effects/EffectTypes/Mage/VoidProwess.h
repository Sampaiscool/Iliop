#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class VoidProwess : public Effect {
    int amount = 0;
public:
    VoidProwess(int value) : amount(value) {}
    VoidProwess() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        // why are you broken ??
        std::cout << "Here is the value: " + value;
        if (amount >= 0) {
            self.applyStatus(std::make_unique<TrueVoidStatus>(2, amount));
        } else {
            
            self.applyStatus(std::make_unique<TrueVoidStatus>(2, value + 2));
        }
    }
};
