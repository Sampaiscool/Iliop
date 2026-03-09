#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class OmegaAnnihilation : public Effect {
    int amount = 0;
public:
    OmegaAnnihilation(int value) : amount(value) {}
    OmegaAnnihilation() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal));

        target.applyStatus(std::make_unique<BleedStatus>(5, finalVal));
        target.applyStatus(std::make_unique<StunStatus>(1, 0));
        target.applyStatus(std::make_unique<DefenceDownStatus>(3, finalVal));
        target.applyStatus(std::make_unique<DamageDownStatus>(3, finalVal));

        if (!self.isTransformed) {
            self.transform(target);
        } else {
            self.transformTime += 5;
        }

        self.applyStatus(std::make_unique<DamageUpStatus>(3, finalVal));
        self.applyStatus(std::make_unique<DefenceUpStatus>(3, finalVal));
    }
};
