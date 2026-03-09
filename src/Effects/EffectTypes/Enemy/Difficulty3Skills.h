#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class MagicSprightSkill : public Effect {
    int amount = 0;
public:
    MagicSprightSkill(int value) : amount(value) {}
    MagicSprightSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.takeDamage(self.getModifiedDamage(finalVal * 2));

        target.applyStatus(std::make_unique<OverloadStatus>(2, finalVal));
    }
};

class LihmSkill : public Effect {
    int amount = 0;
public:
    LihmSkill(int value) : amount(value) {}
    LihmSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        self.applyStatus(std::make_unique<DefenceUpStatus>(3, finalVal * 2));
        self.addShield(finalVal * 2);

        if (rand() % 100 < 50) {
            target.takeDamage(self.getModifiedDamage(finalVal));
        }

    }
};

class NutouSkill : public Effect {
    int amount = 0;
public:
    NutouSkill(int value) : amount(value) {}
    NutouSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        target.takeDamage(self.getModifiedDamage(finalVal));

        target.applyStatus(std::make_unique<DefenceDownStatus>(2, finalVal));
        target.applyStatus(std::make_unique<DamageDownStatus>(2, finalVal));
    }
};

class MushroomManSkill : public Effect {
    int amount = 0;
public:
    MushroomManSkill(int value) : amount(value) {}
    MushroomManSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        target.applyStatus(std::make_unique<BleedStatus>(5, finalVal));
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, finalVal));
        target.takeDamage(self.getModifiedDamage(finalVal));
    }
};

class BiyumiSkill : public Effect {
    int amount = 0;
public:
    BiyumiSkill(int value) : amount(value) {}
    BiyumiSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.addShield(finalVal * 3);
        self.applyStatus(std::make_unique<BlessedStatus>(3, finalVal));

        target.takeDamage(self.getModifiedDamage(finalVal));
    }
};
