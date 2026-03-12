#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class EldritchBlast : public Effect {
    int amount = 0;
public:
    EldritchBlast(int value) : amount(value) {}
    EldritchBlast() = default;

    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = self.getModifiedDamage(value) + self.corruption.current;
        target.takeDamage(finalVal);

        self.gainCorruption(2);
    }
};
