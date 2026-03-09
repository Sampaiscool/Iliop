#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class DivineArrowFusion : public Effect {
    int amount = 0;
public:
    DivineArrowFusion(int value) : amount(value) {}
    DivineArrowFusion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal));

        target.applyStatus(std::make_unique<BleedStatus>(3, finalVal));
        self.applyStatus(std::make_unique<BlessedStatus>(2, finalVal));

        if (self.isTransformed) {
            target.applyStatus(std::make_unique<JudgedStatus>(2, finalVal));
        }
    }
};
