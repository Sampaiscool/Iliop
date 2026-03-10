#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class SoulGathering : public Effect {
    int amount = 0;
public:
    SoulGathering(int value) : amount(value) {}
    SoulGathering() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        for (auto& s : self.statuses) {
            if (s && s->getType() == StatusType::SoulFragment) {
                s->intensity += finalVal;
                
                if (s->intensity >= 10) {
                    s->intensity = 0;
                    self.applyStatus(std::make_unique<ZombieArmyStatus>(1, 3));
                    self.applyStatus(std::make_unique<SkeletonArmyStatus>(1, 3));
                }
                break;
            }
        }
        
        if (self.corruption.current >= self.transformThreshold) {
            self.heal(finalVal * 2);
        }
    }
};
