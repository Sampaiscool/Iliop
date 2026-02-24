#include "CombatState.h"
#include "AllStatuses.h"
#include "Status.h"
#include "../Effects/Effect.h"
#include <algorithm>

CombatState::CombatState() : transformThreshold(0), transformTime(0), transformGain(0) {}

CombatState::CombatState(Resource h, Resource s, Resource m, Resource c)
    : hp(h), shield(s), mana(m), corruption(c), 
      transformThreshold(0), transformTime(0), transformGain(0) {}

CombatState::~CombatState() = default;

int CombatState::takeDamage(int amount) {
    if (amount <= 0) return 0;
    int finalDamage = amount;

    for (auto& s : statuses) {
        if (s->name == "Void Mark") finalDamage += s->intensity;
        if (s->name == "Defence Up") finalDamage = std::max(0, finalDamage - s->intensity);
        if (s->name == "Defence Down") finalDamage += s->intensity;
    }

    int remainingDamage = finalDamage;
    if (shield.current > 0) {
        int absorbed = std::min(shield.current, remainingDamage);
        shield.current -= absorbed;
        remainingDamage -= absorbed;
    }

    int oldHP = hp.current;
    hp.current = std::max(0, hp.current - remainingDamage);
    int damageDealt = oldHP - hp.current;

    if (damageDealt >= 5) {
        for (auto& s : statuses) {
            if (s->name == "Judged") {
                applyStatus(std::make_unique<DefenceDownStatus>(2, 2));
                break;
            }
        }
    }
    return damageDealt;
}

int CombatState::getModifiedDamage(int baseDamage) {
    int total = baseDamage;
    for (auto& s : statuses) {
        if (s->name == "Arcane Overload") total += s->intensity;
        if (s->name == "Damage Up")       total += s->intensity;
        if (s->name == "Damage Down")     total = std::max(0, total - s->intensity);
    }
    return total;
}

int CombatState::heal(int amount) {
    if (amount <= 0) return 0;
    int oldHP = hp.current;
    hp.current = std::min(hp.max, hp.current + amount);
    return hp.current - oldHP;
}

int CombatState::addShield(int amount) {
    if (amount <= 0) return 0;
    int oldShield = shield.current;
    shield.current = std::min(shield.max, shield.current + amount);
    return shield.current - oldShield;
}

void CombatState::applyStatus(std::unique_ptr<Status> newStatus) {
    if (!newStatus) return;
    for (auto& s : statuses) {
        if (s->name == newStatus->name) {
            s->duration  += newStatus->duration;
            s->intensity += newStatus->intensity;
            return;
        }
    }
    statuses.push_back(std::move(newStatus));
}

void CombatState::endTurn(CombatState& currentActor) {
    for (auto& s : currentActor.statuses) { 
        if (s->name == "Blessed") {
            currentActor.heal(s->intensity); 
        }
    }

    for (auto& s : currentActor.statuses) {
        if (s->name == "Stun") {
            s->duration = 0;
        }
    }
}

void CombatState::updateStatuses() {
    for (auto it = statuses.begin(); it != statuses.end(); ) {
        (*it)->onTurnStart(*this);
        if ((*it)->duration <= 0) it = statuses.erase(it);
        else ++it;
    }
}

void CombatState::transform(CombatState& enemy) {
    isTransformed = true;
    transformTime += transformGain;

    if (onTransform) {
        onTransform->apply(*this, enemy, 0);
    }
}

int CombatState::getTrueVoidMana() const {
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::TrueVoid) {
            return s->intensity;
        }
    }
    return 0;
}

void CombatState::consumeTrueVoid(int amount) {
    for (auto& s : statuses) {
        if (s && s->getType() == StatusType::TrueVoid) {
            s->intensity -= amount;
            return;
        }
    }
}
