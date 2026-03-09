#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class DeathMark : public Effect {
    int amount = 0;
public:
    DeathMark(int value) : amount(value) {}
    DeathMark() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        target.applyStatus(std::make_unique<DeathMarkStatus>(3, finalVal));
        
        if (self.isTransformed) {
            target.applyStatus(std::make_unique<DeathMarkStatus>(3, finalVal * 2));
            target.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
            self.applyStatus(std::make_unique<SoulFragmentStatus>(2, finalVal));
        }
        
        int zombieBonus = self.getZombieArmyIntensity();
        if (zombieBonus > 0) {
            self.applyStatus(std::make_unique<SoulFragmentStatus>(3, zombieBonus / 2));
        }
    }
};
