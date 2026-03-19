#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class VoidEcho : public Effect {
    int amount = 0;
public:
    VoidEcho(int value) : amount(value) {}
    VoidEcho() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        
        int voidMarkCount = 0;
        for (auto& s : target.statuses) {
            if (s && s->getType() == StatusType::VoidMark) {
                voidMarkCount = s->intensity;
                s->intensity += 1;
                break;
            }
        }
        
        if (voidMarkCount > 0) {
            target.takeDamage(voidMarkCount * 3);
            self.applyStatus(std::make_unique<TrueVoidStatus>(2, voidMarkCount));
        } else {
            target.applyStatus(std::make_unique<VoidMarkStatus>(3, 1));
        }
        
        if (self.corruption.current >= self.transformThreshold) {
            self.applyStatus(std::make_unique<OverloadStatus>(2, finalVal * 2));
        }
    }
};
