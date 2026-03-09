#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class PrimordialChaos : public Effect {
    int amount = 0;
public:
    PrimordialChaos(int value) : amount(value) {}
    PrimordialChaos() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        int roll = rand() % 4;
        if (roll == 0) {

            target.takeDamage(self.getModifiedDamage(finalVal * 3));
            target.takeDamage(self.getModifiedDamage(finalVal * 3));
        } else if (roll == 1) {
            self.heal(self.hp.max);
            self.addShield(self.hp.max);
        } else if (roll == 2) {
            self.addShield(finalVal * 10);
            self.applyStatus(std::make_unique<DefenceUpStatus>(5, finalVal * 2));
        } else {
            if (!self.isTransformed) {
                self.transform(target);
            } else {
                self.transformTime += 10;
            }
            self.applyStatus(std::make_unique<BlessedStatus>(5, finalVal));
            self.applyStatus(std::make_unique<DamageUpStatus>(5, finalVal * 2));
        }

        self.applyStatus(std::make_unique<CorruptedVoidStatus>(1, 3));
    }
};
