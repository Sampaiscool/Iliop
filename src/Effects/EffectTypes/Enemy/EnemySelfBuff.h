#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class EnemySelfBuff : public Effect {
    int amount = 0;
public:
    EnemySelfBuff(int value) : amount(value) {}
    EnemySelfBuff() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int val = (value != 0) ? value : amount;
        // enemy heals itself
        self.hp.current += val;
        if (self.hp.current > self.hp.max) self.hp.current = self.hp.max;
        // and gains defence buff
        self.applyStatus(std::make_unique<DefenceUpStatus>(2, 3));
    }
};
