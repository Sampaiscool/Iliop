#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class LeadEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.applyStatus(std::make_unique<LeadStatus>(99, value));
        if (self.getMetalStatusCount() >= 2) {
            self.addCardToHand("Reaction", true);
        }
    }
};
