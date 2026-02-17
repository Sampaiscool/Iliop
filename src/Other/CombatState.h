#pragma once
#include "Resource.h"
#include <memory>

class Effect;

struct CombatState {
    Resource hp;
    Resource shield;
    Resource mana;
    Resource corruption;
    int transformThreshold;
    bool isTransformed = false;
    int transformTime;
    int transformGain;

    std::unique_ptr<Effect> transformationProc;

    void takeDamage(int amount) {
        if (amount <= 0) return;

        int remainingDamage = amount;

        // maybe shield?
        if (shield.current > 0) {
            int absorbed = std::min(shield.current, remainingDamage);
            shield.current -= absorbed;
            remainingDamage -= absorbed;
        }

        hp.current -= remainingDamage;

        if (hp.current < 0) hp.current = 0;
    }

    void heal(int amount) {
        if (amount <= 0) return;
        hp.current += amount;
        if (hp.current > hp.max) hp.current = hp.max;
    }

    void addShield(int amount) {
        if (amount <= 0) return;
        shield.current += amount;
        if (shield.current > shield.max) shield.current = shield.max;
    }
};
