#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class GoldenLance : public Effect {
    int amount = 0;
public:
    GoldenLance(int value) : amount(value) {}
    GoldenLance() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal));

        for (auto& status : target.statuses) {
            if (status->getType() == StatusType::Blessed) {
                if (status->intensity > 5) {
                    self.applyStatus(std::make_unique<TrueVoidStatus>(2, finalVal));
                }
            }
        }

        if (self.isTransformed) {
            self.gainCorruption(finalVal);
        }
    }
};
