#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class HolyZeal : public Effect {
    int amount = 0;
public:
    HolyZeal(int value) : amount(value) {}
    HolyZeal() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        target.applyStatus(std::make_unique<DamageDownStatus>(amount, amount));

    }
};
