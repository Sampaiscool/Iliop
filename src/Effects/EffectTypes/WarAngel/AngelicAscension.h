#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class AngelicAscension : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.applyStatus(std::make_unique<FlightStatus>(3, 1));
        self.applyStatus(std::make_unique<BlessedStatus>(3, 3));
        self.applyStatus(std::make_unique<HolySpiritStatus>(3, 2));
        self.addShield(10);
    }
};
