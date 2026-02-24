#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Ritual : public Effect {
    int amount = 0;
public:
    Ritual(int value) : amount(value) {}
    Ritual() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<BlessedStatus>(3, value));
        } else {
           self.transform(target);
        }
    }
};
