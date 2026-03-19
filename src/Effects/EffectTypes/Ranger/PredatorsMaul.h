#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class PredatorsMaul : public Effect {
    int amount = 0;
public:
    PredatorsMaul(int value) : amount(value) {}
    PredatorsMaul() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        target.takeDamage(self.getModifiedDamage(finalVal));
        target.applyStatus(std::make_unique<BleedStatus>(3, finalVal));
        
        if (self.isTransformed) {
            target.applyStatus(std::make_unique<BleedStatus>(2, finalVal * 2));
            self.heal(finalVal * 2);
        }
        
        for (auto& s : self.statuses) {
            if (s && s->getType() == StatusType::Rage) {
                target.takeDamage(self.getModifiedDamage(s->intensity * 2));
                self.applyStatus(std::make_unique<DamageUpStatus>(1, s->intensity));
                break;
            }
        }
    }
};
