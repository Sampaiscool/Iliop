#pragma once
#include "Status.h"
#include "CombatState.h"

class BleedStatus : public Status {
public:
    BleedStatus(int dur, int intens) { 
        name = "Bleed";
        duration = dur;
        intensity = intens;
    }

    StatusType getType() const override {
        return StatusType::Bleed;
    }

    std::string getDescription() const override {
        return std::to_string(intensity) + " damage end of turn";
    }

    void onTurnStart(CombatState& owner) override {
        owner.takeDamage(intensity);
        duration--;
    }
};
