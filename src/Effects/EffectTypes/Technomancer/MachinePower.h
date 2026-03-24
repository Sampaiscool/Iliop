#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class MachinePower : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        int damage = value;

        for (auto it = self.statuses.begin(); it != self.statuses.end(); ) {
            auto& s = *it;
            if (s && s->getType() == StatusType::MachineUpgrade) {
                std::string upgradeName = s->name;
                int intens = s->intensity;

                if (upgradeName == "Overclocked") {
                    damage += 5 * intens;
                }
                else if (upgradeName == "Bleeding") {
                    target.applyStatus(std::make_unique<BleedStatus>(3, 3 * intens));
                }
                else if (upgradeName == "Shocking") {
                    target.applyStatus(std::make_unique<StunStatus>(1, 0));
                }
                else if (upgradeName == "Vampiric") {
                    self.heal(4 * intens);
                }
                else if (upgradeName == "Armored") {
                    self.addShield(4 * intens);
                }
                else if (upgradeName == "Void") {
                    self.applyStatus(std::make_unique<TrueVoidStatus>(2, intens));
                }
                else if (upgradeName == "Damage Up") {
                    target.applyStatus(std::make_unique<DamageUpStatus>(2, 2 * intens));
                }
                else if (upgradeName == "Defence Up") {
                    target.applyStatus(std::make_unique<DefenceUpStatus>(2, 2 * intens));
                }

                it = self.statuses.erase(it);
            } else {
                ++it;
            }
        }
        target.takeDamage(damage);
    }
};
