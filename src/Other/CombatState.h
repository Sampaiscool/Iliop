#pragma once
#include "Resource.h"
#include "Status.h"
#include <memory>
#include <vector>
#include <utility>

// forward Declarations
class Status;
class Effect;
class Deck;

struct DamageEvent {
    int amount;
    bool isHeal;
    DamageEvent(int amt, bool heal) : amount(amt), isHeal(heal) {}
};

struct PendingCard {
    std::string key;
    bool forceTemporary;
};

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

    // queue of damage/heal events
    mutable std::vector<DamageEvent> damageEvents;

    // pointer to deck for adding cards to hand
    Deck* deck = nullptr;

    // queue of card names to add to hand after resolution
    std::vector<PendingCard> pendingCards;

    // need this for forward declerations
    CombatState();
    CombatState(Resource hp, Resource sh, Resource ma, Resource co);
    ~CombatState();

    CombatState(CombatState&&) noexcept = default;
    CombatState& operator=(CombatState&&) noexcept = default;

    int takeDamage(int amount);
    int heal(int amount);
    int addShield(int amount);
    void draw(int drawAmount);
    void applyStatus(std::unique_ptr<Status> newStatus);
    void updateStatuses();
    void endTurn(CombatState& currentActor);
    int getModifiedDamage(int baseDamage);
    void transform(CombatState& enemy);
    void gainCorruption(int corruptionGain);
    void gainMana(int manaGain);
    int getTrueVoidMana() const;
    void consumeTrueVoid(int amount);
    int getZombieArmyIntensity() const;
    int getSkeletonArmyIntensity() const;
    int getSoulFragmentCount() const;

    void addDamageEvent(int amount, bool isHeal);
    void clearDamageEvents() const { damageEvents.clear(); }

    void addCardToHand(const std::string& cardKey, bool makeTemporary);
    void flushPendingCards();
    bool hasMetalStatus(StatusType metal) const;
    int getMetalStatusCount() const;
    void removeMetalStatus(StatusType metal);
    void removeRandomMetalStatus();
    std::string removeRandomMetalCardFromHand();
    void removeUpgradeStatus();
};
