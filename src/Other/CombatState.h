#pragma once
#include "Resource.h"
#include <memory>
#include <vector>

// forward Declarations
class Status;
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

    std::unique_ptr<Effect> onTransform;
    std::unique_ptr<Effect> onCardPlayProc;
    int passiveValue = 0;

    std::vector<std::unique_ptr<Status>> statuses;

    // need this for forward declerations
    CombatState();
    CombatState(Resource hp, Resource sh, Resource ma, Resource co);
    ~CombatState();

    CombatState(CombatState&&) noexcept = default;
    CombatState& operator=(CombatState&&) noexcept = default;

    int takeDamage(int amount);
    int heal(int amount);
    int addShield(int amount);
    void applyStatus(std::unique_ptr<Status> newStatus);
    void updateStatuses();
    void endTurn(CombatState& currentActor);
    int getModifiedDamage(int baseDamage);
    void transform(CombatState& enemy);
    int getTrueVoidMana() const;
    void consumeTrueVoid(int amount);
};
