#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class DivinePunishment : public Effect {
    int amount = 0;
public:
    DivinePunishment(int value) : amount(value) {}
    DivinePunishment() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int blessedCount = 0;
        for (auto& s : target.statuses) {
            if (s && s->getType() == StatusType::Blessed) {
                blessedCount = s->intensity;
                break;
            }
        }
        
        if (blessedCount > 0) {
            target.takeDamage(self.getModifiedDamage(finalVal * 3 + blessedCount * 4));
            target.applyStatus(std::make_unique<BleedStatus>(2, blessedCount));
        } else {
            target.takeDamage(self.getModifiedDamage(finalVal));
        }
        
        self.applyStatus(std::make_unique<BlessedStatus>(2, finalVal));
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<HolySpiritStatus>(3, finalVal));
            self.applyStatus(std::make_unique<FlightStatus>(2, 1));
        }
    }
};
