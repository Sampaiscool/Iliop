#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Betray : public Effect {
    int amount = 0;
public:
    Betray(int value) : amount(value) {}
    Betray() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        int totalBlessedInt = 0;

        for (auto& status : target.statuses) {
            if (status->getType() == StatusType::Blessed) {
                totalBlessedInt = status->intensity;
            }
        }

        target.takeDamage(self.getModifiedDamage(finalVal * totalBlessedInt));

        int bonusCorruptionGain = (self.isTransformed) ? (2) : 0;

        self.gainCorruption(5 + bonusCorruptionGain);
    }
};
