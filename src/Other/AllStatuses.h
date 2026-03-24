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

class OverloadStatus : public Status {
public:
    OverloadStatus(int dur, int intens) {
        name = "Arcane Overload";
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::Overload; }
    std::string getDescription() const override {
        return "Deal " + std::to_string(intensity) + " more damage\nStart turn: take " + std::to_string(intensity / 2)  + " damage.";
    }

    void onTurnStart(CombatState& owner) override {
        owner.takeDamage(intensity / 2);
        duration--;
    }
};

class VoidMarkStatus : public Status {
public:
    VoidMarkStatus(int dur, int intens = 0) {
        name = "Void Mark";
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::VoidMark; }

    std::string getDescription() const override {
        return "Abyssal Charge: " + std::to_string(intensity) + "/5\n" +
               "For holder: Take 5 dmg\nFor other: gain 5 True Void + draw 5";
    }

    void onTurnStart(CombatState& owner) override {
        duration--;
    }
};

class DefenceUpStatus : public Status {
public:
    DefenceUpStatus(int dur, int intens) {
        name = "Defence Up";
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::DefenceUp; }
    std::string getDescription() const override { return "Reduce incoming damage by " + std::to_string(intensity); }
    void onTurnStart(CombatState& owner) override {
        duration--;
    }
};

class DefenceDownStatus : public Status {
public:
    DefenceDownStatus(int dur, int intens) { name = "Defence Down"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::DefenceDown; }
    std::string getDescription() const override { return "Take " + std::to_string(intensity) + " extra damage."; }
    void onTurnStart(CombatState& owner) override {
        duration--;
    }
};

class DamageDownStatus : public Status {
public:
    DamageDownStatus(int dur, int intens) { name = "Damage Down"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::DamageDown; }
    std::string getDescription() const override { return "Deal " + std::to_string(intensity) + " less damage."; }
    void onTurnStart(CombatState& owner) override {
        duration--;
    }
};

class DamageUpStatus : public Status {
public:
    DamageUpStatus(int dur, int intens) { name = "Damage Up"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::DamageUp; }
    std::string getDescription() const override { return "Deal " + std::to_string(intensity) + " more damage."; }
    void onTurnStart(CombatState& owner) override {
        duration--;
    }
};


class StunStatus : public Status {
public:
    StunStatus(int dur, int intens) { name = "Stun"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Stun; }
    std::string getDescription() const override { return "Skips the next turn."; }
    void onTurnStart(CombatState& owner) override {
        // removes stun aftah 1 turn beacause this is BROKEN!
        duration = 0;
    }
};

class JudgedStatus : public Status {
public:
    JudgedStatus(int dur, int intens) { name = "Judged"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Judged; }
    std::string getDescription() const override {
        return "Taking 5+ damage gives " + std::to_string(intensity) + " Defence Down.";
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class BlessedStatus : public Status {
public:
    BlessedStatus(int dur, int intens) { name = "Blessed"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Blessed; }
    std::string getDescription() const override {
        return "End of turn: Heal " + std::to_string(intensity) + "\nIf your were not at corruption threshhold";
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class TrueVoidStatus : public Status {
public:
    TrueVoidStatus(int dur, int intens) { name = "True Void"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::TrueVoid; }
    std::string getDescription() const override {
        return "You can use this as " + std::to_string(intensity) + " extra mana";
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class CorruptedVoidStatus : public Status {
public:
    CorruptedVoidStatus(int dur, int intens) { name = "Corrupted Void"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::CorruptedVoid; }
    std::string getDescription() const override {
        return "If you play a card while you are corrupted:\n Draw " + std::to_string(intensity) + " cards, lose this at turn start";
    }
    void onTurnStart(CombatState& owner) override { duration = 0; }
};

class LockedStatus : public Status {
public:
    LockedStatus(int dur, int intens) { name = "Locked"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Locked; }
    std::string getDescription() const override {
        return "Deal " + std::to_string(intensity * 2) + " extra damage while you are at max HP";
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class RageStatus : public Status {
public:
    RageStatus(int dur, int intens) { name = "Rage"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Rage; }
    std::string getDescription() const override {
        return "If you take damage while you are under 50% HP:\nGain " + std::to_string(intensity) + " Defence Up + Damage Up\nAnd heal " + std::to_string(intensity * 2);
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class BloodLustStatus : public Status {
public:
    BloodLustStatus(int dur, int intens) { name = "Blood Lust"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::BloodLust; }
    std::string getDescription() const override {
        return "If you play a card while wounded:\nDraw " + std::to_string(intensity) + " cards";
    }
    void onTurnStart(CombatState& owner) override {  }
};

class ZombieArmyStatus : public Status {
public:
    ZombieArmyStatus(int dur, int intens) {
        name = "Zombie Army";
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::ZombieArmy; }
    std::string getDescription() const override {
        return "End of turn: Heal " + std::to_string(std::max(1, intensity / 2));
    }
    void onTurnStart(CombatState& owner) override {
        owner.heal(std::max(1, intensity / 2));
    }
};

class SkeletonArmyStatus : public Status {
public:
    SkeletonArmyStatus(int dur, int intens) {
        name = "Skeleton Army";
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::SkeletonArmy; }
    std::string getDescription() const override {
        return "Each skeleton: +" + std::to_string(intensity) + " damage\nto your necromancer cards";
    }
    void onTurnStart(CombatState& owner) override {
    }
};

class SoulFragmentStatus : public Status {
public:
    SoulFragmentStatus(int dur, int intens) {
        name = "Soul Fragment";
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::SoulFragment; }
    std::string getDescription() const override {
        return "Play card while transformed: +1 Fragment\nAt 10: Summon 3 for both armies\n" + std::to_string(intensity) + "/10";
    }
    void onTurnStart(CombatState& owner) override { 
        
    }
};

class DeathMarkStatus : public Status {
public:
    DeathMarkStatus(int dur, int intens) {
        name = "Death Mark";
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::DeathMark; }
    std::string getDescription() const override {
        return "Take " + std::to_string(intensity * 2) + " extra damage\nWhen marked dies: grant " + std::to_string(intensity) + " Soul Fragment";
    }
    void onTurnStart(CombatState& owner) override {
        duration--;
    }
};

class LeadStatus : public Status {
public:
    LeadStatus(int dur, int intens) { name = "Lead"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Lead; }
    std::string getDescription() const override { return "Heavy metal"; }
    void onTurnStart(CombatState& owner) override { }
};

class GoldStatus : public Status {
public:
    GoldStatus(int dur, int intens) { name = "Gold"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Gold; }
    std::string getDescription() const override { return "Precious metal"; }
    void onTurnStart(CombatState& owner) override {  }
};

class CopperStatus : public Status {
public:
    CopperStatus(int dur, int intens) { name = "Copper"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Copper; }
    std::string getDescription() const override { return "Conductive metal"; }
    void onTurnStart(CombatState& owner) override { }
};

class IronStatus : public Status {
public:
    IronStatus(int dur, int intens) { name = "Iron"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Iron; }
    std::string getDescription() const override { return "Sturdy metal"; }
    void onTurnStart(CombatState& owner) override {  }
};

class MercuryStatus : public Status {
public:
    MercuryStatus(int dur, int intens) { name = "Mercury"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Mercury; }
    std::string getDescription() const override { return "Liquid metal"; }
    void onTurnStart(CombatState& owner) override {  }
};

class SilverStatus : public Status {
public:
    SilverStatus(int dur, int intens) { name = "Silver"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Silver; }
    std::string getDescription() const override { return "Shiny metal"; }
    void onTurnStart(CombatState& owner) override {  }
};

class TinStatus : public Status {
public:
    TinStatus(int dur, int intens) { name = "Tin"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Tin; }
    std::string getDescription() const override { return "Soft metal"; }
    void onTurnStart(CombatState& owner) override {  }
};

class RegenerationStatus : public Status {
public:
    RegenerationStatus(int dur, int intens) { name = "Regeneration"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Regeneration; }
    std::string getDescription() const override { return "Heal " + std::to_string(intensity) + " at end of turn"; }
    void onTurnStart(CombatState& owner) override { 
        owner.heal(intensity);
        duration--; 
    }
};

class WeakerCreatorStatus : public Status {
public:
    WeakerCreatorStatus(int dur, int intens) { name = "Weaker Creator"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::WeakerCreator; }
    std::string getDescription() const override { return "Cards played: " + std::to_string(intensity) + "/7\nAt 7: Transform"; }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class SupremeMachineStatus : public Status {
public:
    SupremeMachineStatus(int dur, int intens) { name = "Supreme Machine"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::SupremeMachine; }
    std::string getDescription() const override { return "Statuses received: " + std::to_string(intensity) + "/4\nAt 4: True Void + draw"; }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class MachineUpgradeStatus : public Status {
public:
    MachineUpgradeStatus(int dur, int intens, std::string upgradeName = "Machine")
        : upgradeType(upgradeName) {
        name = upgradeType;
        duration = dur;
        intensity = intens;
    }
    StatusType getType() const override { return StatusType::MachineUpgrade; }
    std::string getDescription() const override { 
        if (upgradeType == "Overclocked") return "Machine Power deals +"  + std::to_string(intensity * 5) + " damage";
        if (upgradeType == "Bleeding")    return "Machine Power applies " + std::to_string(intensity * 3) + " Bleed";
        if (upgradeType == "Shocking")    return "Machine Power applies " + std::to_string(intensity)     + " Stun";
        if (upgradeType == "Vampiric")    return "Machine Power gives "   + std::to_string(intensity)     + " Heal";
        if (upgradeType == "Armored")     return "Machine Power gives "   + std::to_string(intensity)     + " Shield";
        if (upgradeType == "True Void")   return "Machine Power gives "   + std::to_string(intensity)     + " True Void";
        if (upgradeType == "Damage Up")   return "Machine Power gives "   + std::to_string(intensity)     + " Damage Up";
        if (upgradeType == "Defence Up")  return "Machine Power gives "   + std::to_string(intensity)     + " Defence Up";
        return "Machine Power: " + upgradeType;
    }
    void onTurnStart(CombatState& owner) override { duration--; }
    std::string getUpgradeType() const { return upgradeType; }
private:
    std::string upgradeType;
};

class FlightStatus : public Status {
public:
    FlightStatus(int dur, int intens) { name = "Flight"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::Flight; }
    std::string getDescription() const override { return  "Reduce damage taken to " + std::to_string(100 / (intensity + 1)) + "%\n" +
                                                          "Gain +" + std::to_string(intensity) + " corrpution from playing cards"; }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class HolySpiritStatus : public Status {
public:
    HolySpiritStatus(int dur, int intens) { name = "Holy Spirit"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::HolySpirit; }
    std::string getDescription() const override { return "If you play a card: \nGive yourself and the enemy " + std::to_string(intensity) + " Blessed"; }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class ForbiddenDropletStatus : public Status {
public:
    ForbiddenDropletStatus(int dur, int intens) { name = "Forbidden Droplet"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::ForbiddenDroplet; }
    std::string getDescription() const override { return "If you play a card while corrupted: Transform"; }
    void onTurnStart(CombatState& owner) override {  }
};

class CatalystStatus : public Status {
public:
    CatalystStatus(int dur, int intens) { 
        name = "Catalyst"; 
        duration = dur; 
        intensity = intens; 
    }
    StatusType getType() const override { return StatusType::Catalyst; }
    std::string getDescription() const override {
        return "Heal and Shield +" + std::to_string(intensity * 2);
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class PotionStatus : public Status {
public:
    PotionStatus(int dur, int intens) { 
        name = "Potion"; 
        duration = dur; 
        intensity = intens; 
    }
    StatusType getType() const override { return StatusType::Potion; }
    std::string getDescription() const override {
        return "Click to heal " + std::to_string(intensity * 3);
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

class ElixirStatus : public Status {
public:
    ElixirStatus(int dur, int intens) { 
        name = "Elixir"; 
        duration = dur; 
        intensity = intens; 
    }
    StatusType getType() const override { return StatusType::Elixir; }
    std::string getDescription() const override {
        return "Cards cost -1\nClick 5+ to Transform";
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};
