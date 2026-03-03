#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Jump : public Effect {
    int amount = 0;
public:
    Jump(int value) : amount(value) {}
    Jump() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        if (self.isTransformed) {
            target.takeDamage(self.getModifiedDamage(finalVal));
            self.applyStatus(std::make_unique<DefenceUpStatus>(2, finalVal));
        } else {
            self.addShield(finalVal);
            self.applyStatus(std::make_unique<DefenceUpStatus>(2, finalVal / 2));
        }
    }
};
