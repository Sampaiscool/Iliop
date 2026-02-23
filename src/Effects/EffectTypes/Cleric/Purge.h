#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Purge : public Effect {
    int amount = 0;
public:
    Purge(int value) : amount(value) {}
    Purge() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int statusCount = target.statuses.size();
        if (statusCount > 0) {
            target.takeDamage(self.getModifiedDamage(value * statusCount));
        }
    }
};
