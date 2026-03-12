#pragma once
#include <iostream>
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Savior : public Effect {
    int amount = 0;
public:
    Savior(int value) : amount(value) {}
    Savior() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        if (self.isTransformed) {
            finalVal *= 2;
            self.gainCorruption(finalVal);
        }
        
        self.applyStatus(std::make_unique<HolySpiritStatus>(5, finalVal));
        self.gainMana(finalVal);

        self.draw(finalVal);
    }
};
