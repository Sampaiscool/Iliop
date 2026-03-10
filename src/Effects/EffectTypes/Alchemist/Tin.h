#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class TinEffect : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.applyStatus(std::make_unique<TinStatus>(99, value));
        if (self.getMetalStatusCount() >= 2) {
            self.addCardToHand("Reaction");
        }
    }
};
