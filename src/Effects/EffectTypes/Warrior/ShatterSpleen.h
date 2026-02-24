#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class ShatterSpleen : public Effect {
    int amount = 0;
public:
    ShatterSpleen(int value) : amount(value) {}
    ShatterSpleen() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int defenceUpGain = (std::max((self.shield.current / 10), 1));
        self.applyStatus(std::make_unique<DefenceUpStatus>(1, defenceUpGain));
        if (self.shield.current == self.shield.max) {
            int damage = self.getModifiedDamage(value) * 2;
            target.takeDamage(damage);
        }

        if (self.isTransformed) {
            target.applyStatus(std::make_unique<DamageDownStatus>(3, defenceUpGain));
        }
    }
};
