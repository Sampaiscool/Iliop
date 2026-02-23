#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class PrimalArrowEffect : public Effect {
    int amount = 0;
public:
    PrimalArrowEffect(int value) : amount(value) {}
    PrimalArrowEffect() = default;

    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        if (!self.isTransformed) {
            target.takeDamage(self.getModifiedDamage(finalVal));
            self.isTransformed = true;
            self.transformTime += self.transformGain;
        } else {
            for (int i = 0; i < 5; ++i) {
                target.takeDamage(self.getModifiedDamage(finalVal / 2));
                if ((rand() % 100) < 25) {
                    target.applyStatus(std::make_unique<BleedStatus>(2, 3));
                }
            }
        }
    }
};
