#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class LittleGremlinSkill : public Effect {
    int amount = 0;
public:
    LittleGremlinSkill(int value) : amount(value) {}
    LittleGremlinSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;

        self.applyStatus(std::make_unique<DamageUpStatus>(1, finalVal));
        target.takeDamage(self.getModifiedDamage(finalVal));
    }
};

class AngerBotSkill : public Effect {
    int amount = 0;
public:
    AngerBotSkill(int value) : amount(value) {}
    AngerBotSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        int selfDamage = self.hp.max / 10;

        self.takeDamage(selfDamage);

        target.takeDamage(self.getModifiedDamage(finalVal));
    }
};

class PuthSkill : public Effect {
    int amount = 0;
public:
    PuthSkill(int value) : amount(value) {}
    PuthSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        target.takeDamage(self.getModifiedDamage(finalVal));

        self.heal(finalVal);
    }
};

class WarChiefSkill : public Effect {
    int amount = 0;
public:
    WarChiefSkill(int value) : amount(value) {}
    WarChiefSkill() = default;
    void apply(CombatState& self, CombatState& target, int value) override {
        int finalVal = (value != 0) ? value : amount;
        self.applyStatus(std::make_unique<DamageUpStatus>(2, finalVal));
        self.applyStatus(std::make_unique<DefenceUpStatus>(2, finalVal));

        target.applyStatus(std::make_unique<DamageDownStatus>(2, finalVal));
        target.takeDamage(self.getModifiedDamage(finalVal));
    }
};
