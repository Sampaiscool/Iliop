#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class RottingBite : public Effect {
    int amount = 0;
public:
    RottingBite(int value) : amount(value) {}
    RottingBite() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int zombieBonus = self.getZombieArmyIntensity();
        int totalDamage = self.getModifiedDamage(finalVal + zombieBonus);
        
        target.takeDamage(totalDamage);
        target.applyStatus(std::make_unique<BleedStatus>(2, zombieBonus / 2 + 1));
        
        self.heal(zombieBonus);
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<ZombieArmyStatus>(99, 2));
            target.applyStatus(std::make_unique<DeathMarkStatus>(3, zombieBonus / 3 + 1));
        }
    }
};
