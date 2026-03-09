#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class FatedTravelerSkill : public Effect {
    int amount = 0;
public:
    FatedTravelerSkill(int value) : amount(value) {}
    FatedTravelerSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        target.takeDamage(self.getModifiedDamage(finalVal * 2));

        target.applyStatus(std::make_unique<StunStatus>(1, 0));
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, finalVal));
    }
};

class KazanSkill : public Effect {
    int amount = 0;
public:
    KazanSkill(int value) : amount(value) {}
    KazanSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        target.takeDamage(self.getModifiedDamage(finalVal * 2));

        target.applyStatus(std::make_unique<BleedStatus>(3, finalVal));

        self.heal(finalVal);
    }
};

class DeathMarcherSkill : public Effect {
    int amount = 0;
public:
    DeathMarcherSkill(int value) : amount(value) {}
    DeathMarcherSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal));
        target.takeDamage(self.getModifiedDamage(finalVal));

        target.applyStatus(std::make_unique<DamageDownStatus>(3, finalVal));
        target.applyStatus(std::make_unique<DefenceDownStatus>(3, finalVal));
        target.applyStatus(std::make_unique<BleedStatus>(2, finalVal));
    }
};

class MentaSkill : public Effect {
    int amount = 0;
public:
    MentaSkill(int value) : amount(value) {}
    MentaSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal));

        target.applyStatus(std::make_unique<DamageDownStatus>(2, finalVal));
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, finalVal));
        target.applyStatus(std::make_unique<JudgedStatus>(2, finalVal));
    }
};

class CursedKingSkill : public Effect {
    int amount = 0;
public:
    CursedKingSkill(int value) : amount(value) {}
    CursedKingSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal * 2));

        target.applyStatus(std::make_unique<JudgedStatus>(3, finalVal));
        target.applyStatus(std::make_unique<DamageDownStatus>(3, finalVal));
        target.applyStatus(std::make_unique<DefenceDownStatus>(3, finalVal));

        self.heal(finalVal);
    }
};
