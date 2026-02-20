#pragma once
#include <string>

enum class StatusType {
    Bleed,
    Stun,
    DamageDown,
    DamageUp,
    DefenceDown,
    DefenceUp,
};

struct CombatState;

struct Status {
    std::string name;
    int duration;
    int intensity;

    virtual void onTurnStart(CombatState& owner) = 0;
    virtual StatusType getType() const = 0;
    virtual std::string getDescription() const = 0;
    virtual ~Status() = default;
};
