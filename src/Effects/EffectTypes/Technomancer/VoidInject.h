#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class VoidInject : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        // Apply Void upgrade - Machine Power gains True Void
        self.applyStatus(std::make_unique<MachineUpgradeStatus>(3, value, "Void"));
    }
};
