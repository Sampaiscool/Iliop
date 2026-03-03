#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class ExistentialCrisis : public Effect {
    int amount = 0;
public:
    ExistentialCrisis(int value) : amount(value) {}
    ExistentialCrisis() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        // If enemy > 50% HP: INSTANT KILL!
        if (target.hp.current > target.hp.max / 2) {
            target.takeDamage(999);
            // Apply void mark for style
            target.applyStatus(std::make_unique<VoidMarkStatus>(10, 5));
        } else {
            // Else: FULLY HEAL + MAX SHIELD!
            self.heal(self.hp.max);
            self.addShield(self.hp.max);
            self.applyStatus(std::make_unique<DefenceUpStatus>(10, 50));
            self.applyStatus(std::make_unique<DamageUpStatus>(10, 50));
        }
        
        // Always gain True Void regardless
        self.applyStatus(std::make_unique<TrueVoidStatus>(5, 10));
    }
};
