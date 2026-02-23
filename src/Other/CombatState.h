#pragma once
#include "Resource.h"
#include <memory>
#include <vector>

// Forward Declarations: Tells the compiler these exist without including them
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

    std::unique_ptr<Effect> transformationProc;
    std::vector<std::unique_ptr<Status>> statuses;

    CombatState();
    CombatState(Resource hp, Resource sh, Resource ma, Resource co);
    ~CombatState();

    CombatState(CombatState&&) noexcept = default;
    CombatState& operator=(CombatState&&) noexcept = default;

    // Function Declarations only
    int takeDamage(int amount);
    int heal(int amount);
    int addShield(int amount);
    void applyStatus(std::unique_ptr<Status> newStatus);
    void updateStatuses();
    void endTurn(CombatState& currentActor);
    int getModifiedDamage(int baseDamage);
};
