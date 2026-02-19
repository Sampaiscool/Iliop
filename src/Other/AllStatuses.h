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

    void onTurnStart(CombatState& owner) override {
        owner.takeDamage(intensity);
        duration--;
    }
};
