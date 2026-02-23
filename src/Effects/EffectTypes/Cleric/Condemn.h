#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Condemn : public Effect {
    int amount = 0;
public:
    Condemn(int value) : amount(value) {}
    Condemn() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        target.applyStatus(std::make_unique<BleedStatus>(2, std::max(value / 3, 1)));
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, std::max(value / 3, 1)));


        if (self.isTransformed) {
            target.applyStatus(std::make_unique<JudgedStatus>(3, std::max(value / 5, 1)));
            self.applyStatus(std::make_unique<BleedStatus>(3, std::max(value / 5, 1)));
        }
    }
};
