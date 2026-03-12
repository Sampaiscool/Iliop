#pragma once
#include <string>

enum class StatusType {
    Overload,
    Bleed,
    Stun,
    DamageDown,
    DamageUp,
    DefenceDown,
    DefenceUp,
    VoidMark,
    TrueVoid,
    CorruptedVoid,
    Judged,
    Blessed,
    Locked,
    Rage,
    BloodLust,
    ZombieArmy,
    SkeletonArmy,
    SoulFragment,
    DeathMark,
    Vulnerable,
    Regeneration,
    Lead,
    Gold,
    Copper,
    Iron,
    Mercury,
    Silver,
    Tin,
    WeakerCreator,
    SupremeMachine,
    MachineUpgrade,
    Flight,
    HolySpirit,
    ForbiddenDroplet
};

struct CombatState;

struct Status {
    std::string name;
    int duration = 0;
    int intensity = 0;

    virtual void onTurnStart(CombatState& owner) = 0;
    virtual StatusType getType() const = 0;
    virtual std::string getDescription() const = 0;
    virtual ~Status() = default;
};
