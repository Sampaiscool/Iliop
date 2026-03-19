#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class CorpseExplosion : public Effect {
    int amount = 0;
public:
    CorpseExplosion(int value) : amount(value) {}
    CorpseExplosion() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int zombieBonus = self.getZombieArmyIntensity();
        int skeletonBonus = self.getSkeletonArmyIntensity();
        int armyTotal = zombieBonus + skeletonBonus;
        
        if (armyTotal > 0) {
            target.takeDamage(self.getModifiedDamage(armyTotal * 3 + finalVal));
            target.applyStatus(std::make_unique<BleedStatus>(2, armyTotal));
            
            if (self.isTransformed) {
                self.applyStatus(std::make_unique<SoulFragmentStatus>(3, armyTotal));
                self.heal(armyTotal);
            }
        } else {
            target.takeDamage(self.getModifiedDamage(finalVal));
        }
    }
};
