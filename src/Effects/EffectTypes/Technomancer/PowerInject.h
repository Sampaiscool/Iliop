#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class PowerInject : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        // Apply Power upgrade - Machine Power applies Damage Up
        self.applyStatus(std::make_unique<MachineUpgradeStatus>(3, value, "Damage Up"));
    }
};
