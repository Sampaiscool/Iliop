#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class RaiseDead : public Effect {
    int amount = 0;
public:
    RaiseDead(int value) : amount(value) {}
    RaiseDead() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        self.applyStatus(std::make_unique<ZombieArmyStatus>(1, finalVal));
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<ZombieArmyStatus>(1, finalVal));
            self.heal(finalVal * 2);
        }
    }
};
