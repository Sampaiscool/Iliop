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
        int finalVal = (value != 0) ? value : amount;
        
        int hits = 3;
        if (self.isTransformed) {
            hits = 6;
            for (int i = 0; i < hits; ++i) {
                target.takeDamage(self.getModifiedDamage(finalVal));
            }
            if (rand() % 100 < 50) {
                target.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
            }
        } else {
            for (int i = 0; i < hits; ++i) {
                target.takeDamage(self.getModifiedDamage(finalVal));
            }
        }
    }
};
