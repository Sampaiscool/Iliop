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
    StatusType getType() const override { return StatusType::DamageDown; }
    std::string getDescription() const override { return "Deal " + std::to_string(intensity) + " less damage."; }
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

class RagingBearStatus : public Status {
public:
    RagingBearStatus(int dur, int intens) { name = "Raging bear"; duration = dur; intensity = intens; }
    StatusType getType() const override { return StatusType::RagingBear; }
    std::string getDescription() const override {
        return "If you take damage while you are under 50% HP:\nGain " + std:to_string(intensity) + " Defence Up + Damage Up\nAnd heal " + std::to_string(intensity * 2);
    }
    void onTurnStart(CombatState& owner) override { duration--; }
};

