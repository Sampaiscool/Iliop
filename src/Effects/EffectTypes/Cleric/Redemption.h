#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class Redemption : public Effect {
    int amount = 0;
public:
    Redemption(int value) : amount(value) {}
    Redemption() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int blessedCount = 0;
        for (auto& s : self.statuses) {
            if (s && s->getType() == StatusType::Blessed) {
                blessedCount = s->intensity;
                break;
            }
        }
        
        self.heal(finalVal * 2 + blessedCount * 2);
        
        if (blessedCount > 0) {
            target.takeDamage(blessedCount * 3);
            self.applyStatus(std::make_unique<BleedStatus>(1, blessedCount));
        }
        
        if (self.isTransformed) {
            self.applyStatus(std::make_unique<JudgedStatus>(3, blessedCount));
        }
    }
};
