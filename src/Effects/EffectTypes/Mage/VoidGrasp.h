#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class VoidGrasp : public Effect {
    int amount = 0;
public:
    VoidGrasp(int value) : amount(value) {}
    VoidGrasp() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));

        // apply void-mark, more if transformed
        int markIntensity = self.isTransformed ? 10 : 2;
        target.applyStatus(std::make_unique<VoidMarkStatus>(3, markIntensity));

        // Deal some damage to the shield, can go under 0
        target.shield.current -= (self.corruption.current * 2);
    }
};
