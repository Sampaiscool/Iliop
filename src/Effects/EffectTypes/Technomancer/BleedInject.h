#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class BleedInject : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        self.applyStatus(std::make_unique<MachineUpgradeStatus>(3, value, "Bleeding"));
    }
};
