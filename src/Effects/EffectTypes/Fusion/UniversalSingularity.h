#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class UniversalSingularity : public Effect {
    int amount = 0;
public:
    UniversalSingularity(int value) : amount(value) {}
    UniversalSingularity() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.applyStatus(std::make_unique<VoidMarkStatus>(5, 10));

        self.applyStatus(std::make_unique<TrueVoidStatus>(5, 10));

        self.corruption.current = self.corruption.max;

        self.applyStatus(std::make_unique<OverloadStatus>(3, finalVal));

        if (self.isTransformed) {
            target.takeDamage(50);
            self.heal(30);
            self.applyStatus(std::make_unique<CorruptedVoidStatus>(2, 5));
        }
    }
};
