#pragma once
#include "Resource.h"
#include "Status.h"
#include <memory>
#include <vector>
#include <algorithm>

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
    std::vector<std::unique_ptr<Status>> statuses;

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

    void applyStatus(std::unique_ptr<Status> newStatus) {
      // if it exists add duration else push back
        for (auto& s : statuses) {
            if (s->name == newStatus->name) {
                s->duration += newStatus->duration;
                return;
            }
        }
        statuses.push_back(std::move(newStatus));
    }

    void updateStatuses() {
        for (auto it = statuses.begin(); it != statuses.end(); ) {
            (*it)->onTurnStart(*this);
            if ((*it)->duration <= 0) it = statuses.erase(it);
            else ++it;
        }
    }
};
