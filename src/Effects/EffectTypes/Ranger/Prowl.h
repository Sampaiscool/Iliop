#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Prowl : public Effect {
    int amount = 0;
public:
    Prowl(int value) : amount(value) {}
    Prowl() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        if (self.hp.current >= self.hp.max) {
            // PREDATOR MODE: Perfect health = Execute for double damage + apply bleed
            target.takeDamage(self.getModifiedDamage(finalVal * 2));
            target.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
            self.applyStatus(std::make_unique<DamageUpStatus>(1, finalVal));
        } else {
            // BLOOD HUNT: Wounded = Attack + gain Locked (which boosts damage at max HP)
            target.takeDamage(self.getModifiedDamage(finalVal));
            self.applyStatus(std::make_unique<LockedStatus>(1, finalVal));
            self.heal(finalVal);
            
            // THE COOL PART: Draw cards when hunting! (Like mage's corruption draw)
            // This is handled elsewhere - the real power is in the identity
        }
    }
};
