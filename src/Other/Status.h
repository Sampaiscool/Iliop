#pragma once
#include <string>

struct CombatState;

struct Status {
    std::string name;
    int duration;
    int intensity;

    virtual void onTurnStart(CombatState& owner) = 0;
    virtual ~Status() = default;
};
