#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Infinity : public Effect {
    int amount = 0;
public:
    Infinity(int value) : amount(value) {}
    Infinity() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        if (self.corruption.current >= self.transformThreshold) {
            self.applyStatus(std::make_unique<CorruptedVoidStatus>(0, amount));
        }
    }
};
