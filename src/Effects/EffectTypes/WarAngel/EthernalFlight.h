#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class EthernalFlight : public Effect {
    int amount = 0;
public:
    EthernalFlight(int value) : amount(value) {}
    EthernalFlight() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.applyStatus(std::make_unique<FlightStatus>(3, finalVal));

        self.gainCorruption(finalVal);
    }
};
