#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class StatusDrive : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        // yup WIP
    }
};
