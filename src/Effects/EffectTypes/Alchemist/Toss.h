#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"
#include <string>

class Toss : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        // remove card and catch name
        std::string metal = self.removeRandomMetalCardFromHand();
        int extraDamage = 0;
        int bonusCorruption = 0;

        // determine extra damage based on metal
        if (!metal.empty()) {
            if      (metal == "Lead")     { extraDamage = 3;   bonusCorruption = 1; }
            else if (metal == "Gold")     { extraDamage = 4;   bonusCorruption = 4; }
            else if (metal == "Iron")     { extraDamage = 3;   bonusCorruption = 1; }
            else if (metal == "Silver")   { extraDamage = 3;   bonusCorruption = 1; }
            else if (metal == "Mercury")  { extraDamage = 2;   bonusCorruption = 4; }
            else if (metal == "Copper")   { extraDamage = 2;   bonusCorruption = 2; }
            else if (metal == "Tin")      { extraDamage = 1;   bonusCorruption = 2; }
        }

        // base damage starts with card value + metal bonus
        int finalValue = value + extraDamage;

        // corruption check
        if (self.corruption.current >= self.transformThreshold) {
            finalValue += bonusCorruption;
        }

        // apply final damage with modifiers
        target.takeDamage(self.getModifiedDamage(finalValue));
    }
};
