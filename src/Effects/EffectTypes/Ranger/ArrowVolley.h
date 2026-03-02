#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class ArrowVolley : public Effect {
    int amount = 0;
public:
    ArrowVolley(int value) : amount(value) {}
    ArrowVolley() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        
    }
};
