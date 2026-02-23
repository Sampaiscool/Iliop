#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BlindingLight : public Effect {
    int amount = 0;
public:
    BlindingLight(int value) : amount(value) {}
    BlindingLight() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        bool hasTwoStatuses = (target.statuses.size() >= 2);

        if (hasTwoStatuses) {
            target.applyStatus(std::make_unique<StunStatus>(1, 3));
        }

        target.applyStatus(std::make_unique<DamageDownStatus>(3, 2));

        int multiplier = hasTwoStatuses ? 2 : 1;

        target.takeDamage(self.getModifiedDamage(value * multiplier));
    }
};
