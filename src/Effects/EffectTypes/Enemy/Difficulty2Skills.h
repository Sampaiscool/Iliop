#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class FatGremlinSkill : public Effect {
    int amount = 0;
public:
    FatGremlinSkill(int value) : amount(value) {}
    FatGremlinSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.heal(finalVal);
        self.applyStatus(std::make_unique<BlessedStatus>(2, finalVal));
    }
};

class BungusSkill : public Effect {
    int amount = 0;
public:
    BungusSkill(int value) : amount(value) {}
    BungusSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        target.takeDamage(self.getModifiedDamage(finalVal));

        target.applyStatus(std::make_unique<BleedStatus>(3, finalVal));
    }
};

class CultistSkill : public Effect {
    int amount = 0;
public:
    CultistSkill(int value) : amount(value) {}
    CultistSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        target.takeDamage(self.getModifiedDamage(finalVal));

        self.heal(finalVal / 2);

        target.applyStatus(std::make_unique<DamageDownStatus>(2, finalVal / 2));
    }
};

class GiggyGrassSkill : public Effect {
    int amount = 0;
public:
    GiggyGrassSkill(int value) : amount(value) {}
    GiggyGrassSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        for (int i = 0; i < 3; i++) {
            target.takeDamage(self.getModifiedDamage(finalVal / 2));
        }

        target.applyStatus(std::make_unique<DefenceDownStatus>(2, finalVal));
    }
};

class BulletJuniorSkill : public Effect {
    int amount = 0;
public:
    BulletJuniorSkill(int value) : amount(value) {}
    BulletJuniorSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        self.addShield(finalVal);
        int piercingDamage = self.getModifiedDamage(finalVal);
        target.takeDamage(piercingDamage);
    }
};
