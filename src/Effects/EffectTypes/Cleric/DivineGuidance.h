#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class DivineGuidance : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.heal(5);
        self.applyStatus(std::make_unique<BlessedStatus>(5, 5));
    }
};
