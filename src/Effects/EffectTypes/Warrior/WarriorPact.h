#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class WarriorPact : public Effect {
    int amount = 0;
public:
    WarriorPact(int value) : amount(value) {}
    WarriorPact() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        self.takeDamage(self.getModifiedDamage(value));
        self.mana.current =
            std::min(self.mana.current + 2,
                    self.mana.max);
    }
};
