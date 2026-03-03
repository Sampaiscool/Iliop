#pragma once
#include "Effect.h"
#include <vector>
#include <memory>

class FusionEffect : public Effect {
    std::vector<std::unique_ptr<Effect>> effects;
    int bonusDamage = 0;
    int bonusHeal = 0;
    int bonusShield = 0;

public:
    FusionEffect(int damage, int heal, int shield) 
        : bonusDamage(damage), bonusHeal(heal), bonusShield(shield) {}
    FusionEffect() = default;

    void addEffect(std::unique_ptr<Effect> e) {
        effects.push_back(std::move(e));
    }

    void apply(CombatState& self, CombatState& target, int value) override {
        for (auto& e : effects) {
            if (e) e->apply(self, target, value);
        }
        
        if (bonusDamage > 0) {
            target.takeDamage(self.getModifiedDamage(bonusDamage));
        }
        if (bonusHeal > 0) {
            self.heal(bonusHeal);
        }
        if (bonusShield > 0) {
            self.addShield(bonusShield);
        }
    }
};
