#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class DivineAegisEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.addShield(value);
        self.heal(value / 2);
        self.applyStatus(std::make_unique<DefenceUpStatus>(2, 2));
        
        if (self.isTransformed) {
            self.addShield(value);
            self.applyStatus(std::make_unique<RegenerationStatus>(2, 3));
        }
    }
};
