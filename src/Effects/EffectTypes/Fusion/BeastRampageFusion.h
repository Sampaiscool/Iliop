#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BeastRampageFusion : public Effect {
    int amount = 0;
public:
    BeastRampageFusion(int value) : amount(value) {}
    BeastRampageFusion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal * 2));

        self.applyStatus(std::make_unique<DefenceUpStatus>(2, finalVal));
        self.applyStatus(std::make_unique<DamageUpStatus>(2, finalVal));

        if (self.isTransformed) {
            target.takeDamage(self.getModifiedDamage(finalVal));
            target.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
            self.transformTime += 2;
        }
    }
};
