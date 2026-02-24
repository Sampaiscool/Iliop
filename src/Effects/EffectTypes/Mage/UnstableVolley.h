#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class UnstableVolley : public Effect {
    int amount = 0;
public:
    UnstableVolley(int value) : amount(value) {}
    UnstableVolley() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        target.takeDamage(self.getModifiedDamage(value));

        if (!self.isTransformed) {
            // gain corruption, if you are at max, transform
            self.corruption.current =
            std::min(self.corruption.current + 2,
                    self.corruption.max);
            if (self.corruption.current >= self.transformThreshold) {
                self.transform(target);
            }
        } else {
            // if transformed, gain overload
            self.applyStatus(std::make_unique<OverloadStatus>(3, 4));
        }
    }
};
