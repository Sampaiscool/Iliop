#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class SystemCrash : public Effect {
    int amount = 0;
public:
    SystemCrash(int value) : amount(value) {}
    SystemCrash() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int upgradeCount = 0;
        for (auto& s : self.statuses) {
            if (s && s->getType() == StatusType::MachineUpgrade) {
                upgradeCount += s->intensity;
            }
        }
        
        target.takeDamage(self.getModifiedDamage(finalVal * 2 + upgradeCount * 3));
        
        if (upgradeCount >= 3) {
            target.applyStatus(std::make_unique<StunStatus>(1, 1));
        }
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<SupremeMachineStatus>(3, finalVal));
        }
    }
};
