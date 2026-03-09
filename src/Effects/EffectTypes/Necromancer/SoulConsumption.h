#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class SoulConsumption : public Effect {
    int amount = 0;
public:
    SoulConsumption(int value) : amount(value) {}
    SoulConsumption() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        int soulCount = self.getSoulFragmentCount();
        
        if (soulCount > 0) {
            int consumeAmount = std::min(soulCount, finalVal);
            
            int bonusDamage = consumeAmount * 2;
            target.takeDamage(self.getModifiedDamage(bonusDamage));
            target.applyStatus(std::make_unique<BleedStatus>(1, consumeAmount));
            
            if (consumeAmount >= 3) {
                self.applyStatus(std::make_unique<ZombieArmyStatus>(1, 1));
                self.applyStatus(std::make_unique<SkeletonArmyStatus>(1, 1));
            }
            
            for (auto& s : self.statuses) {
                if (s && s->getType() == StatusType::SoulFragment) {
                    s->intensity -= consumeAmount;
                    if (s->intensity <= 0) {
                        s->duration = 0;
                    }
                    break;
                }
            }
        }
        
        self.applyStatus(std::make_unique<SoulFragmentStatus>(1, 1));
    }
};
