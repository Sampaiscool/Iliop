#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class CatalystBomb : public Effect {
    int amount = 0;
public:
    CatalystBomb(int value) : amount(value) {}
    CatalystBomb() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int catalyst = self.getCatalystIntensity();
        
        if (catalyst > 0) {
            int explosion = finalVal * 3 + catalyst * 4;
            target.takeDamage(self.getModifiedDamage(explosion));
            target.applyStatus(std::make_unique<DefenceDownStatus>(2, catalyst));
            
            for (auto& s : self.statuses) {
                if (s && s->getType() == StatusType::Catalyst) {
                    s->duration = 0;
                    break;
                }
            }
        } else {
            target.takeDamage(self.getModifiedDamage(finalVal));
            self.applyStatus(std::make_unique<CatalystStatus>(2, 1));
        }
        
        self.applyStatus(std::make_unique<PotionStatus>(2, finalVal));
    }
};
