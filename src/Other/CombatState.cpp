#include "CombatState.h"
#include "AllStatuses.h"
#include "Status.h"
#include "../Effects/Effect.h"
#include "../Card/Card.h"
#include "../Deck/Deck.h"
#include "../Managers/CardFactory.h"
#include <algorithm>

CombatState::CombatState() : transformThreshold(0), transformTime(0), transformGain(0) {}

CombatState::CombatState(Resource h, Resource s, Resource m, Resource c)
    : hp(h), shield(s), mana(m), corruption(c),
      transformThreshold(0), transformTime(0), transformGain(0) {}

CombatState::~CombatState() = default;

/// @brief deals damage to the state
/// @param amount the amount of total damage to deal
/// @return the actual damage dealt
int CombatState::takeDamage(int amount) {
    if (amount <= 0) return 0;
    int finalDamage = amount;

    // applies modifiers of the damage from statusses
    for (auto& s : statuses) {
        if (s->name == "Death Mark") finalDamage += s->intensity * 2;
        if (s->name == "Void Mark") finalDamage += s->intensity;
        if (s->name == "Defence Up") finalDamage = std::max(0, finalDamage - s->intensity);
        if (s->name == "Defence Down") finalDamage += s->intensity;
        if (s->name == "Flight") finalDamage /= (s->intensity + 1);
    }

    int remainingDamage = finalDamage;

    if (shield.current > 0) {
        int absorbed = std::min(shield.current, remainingDamage);
        shield.current -= absorbed;
        remainingDamage -= absorbed;
        if (absorbed > 0) damageEvents.emplace_back(absorbed, false, true);
    }

    int oldHP = hp.current;
    hp.current = std::max(0, hp.current - remainingDamage);
    int damageDealt = oldHP - hp.current;

    // add damage event
    if (damageDealt > 0) {
        damageEvents.emplace_back(damageDealt, false, false);
    }

    // on damage effects
    if (damageDealt > 0) {
        for (auto& s : statuses) {
            if (s->name == "Raging Bear") {
                if (hp.current <= (hp.max / 2)) {
                    applyStatus(std::make_unique<DefenceUpStatus>(2, s->intensity));
                    applyStatus(std::make_unique<DamageUpStatus>(2, s->intensity));
                }
            }
            if (s->name == "Judged") {
                if (damageDealt >= 5) {
                    applyStatus(std::make_unique<DefenceDownStatus>(2, 2));
                }
            }
        }
    }


    
    return damageDealt;
}

/// @brief gets the modified damage based on status effects
/// @param baseDamage the damage to amplify or reduce
/// @return the total modified damage
int CombatState::getModifiedDamage(int baseDamage) {
    int total = baseDamage;
    for (auto& s : statuses) {
        if (s->name == "Arcane Overload") total += s->intensity;
        if (s->name == "Damage Up")       total += s->intensity;
        if (s->name == "Damage Down")     total = std::max(0, total - s->intensity);
        if (s->name == "Locked")          if (hp.current == hp.max) total += ((s->intensity) * 2);
    }
    return total;
}

/// @brief heals the state
/// @param amount the amount to heal by
/// @return the amount healed
int CombatState::heal(int amount) {
    if (amount <= 0) return 0;
    int oldHP = hp.current;
    hp.current = std::min(hp.max, hp.current + amount);
    int healed = hp.current - oldHP;
    if (healed > 0) {
        damageEvents.emplace_back(healed, true, false);
    }
    return healed;
}

/// @brief gives a shield to the state
/// @param amount the amount of shield to give
/// @return the shield amount it gained
int CombatState::addShield(int amount) {
    if (amount <= 0) return 0;
    int oldShield = shield.current;
    shield.current = std::min(shield.max, shield.current + amount);
    int gained = shield.current - oldShield;
    if (gained > 0) {
        damageEvents.emplace_back(gained, false, true);
    }
    return gained;
}

/// @brief draws a cards from the deck
/// @param drawAmount the amount to draw from the deck
void CombatState::draw(int drawAmount) {
    if (deck) {
        deck->drawCard(drawAmount);
    }
}

/// @brief applies a status to the state, can also incease a status if it already has it
/// @param newStatus the status to apply
void CombatState::applyStatus(std::unique_ptr<Status> newStatus) {
    if (!newStatus) return;

    // suprime machine increment
    for (auto& s : statuses) {
        if (s->getType() == StatusType::SupremeMachine) {
            s->intensity += 1;
            break;
        }
    }
 
    // if the status already exists, refresh duration and add intensity
    for (auto& s : statuses) {
        if (s->name == newStatus->name) {
            s->duration  += newStatus->duration;
            s->intensity += newStatus->intensity;
            return;
        }
    }
    statuses.push_back(std::move(newStatus));
}

/// @brief ends the turn of the combat state
/// @param currentActor which state's turn it is
void CombatState::endTurn(CombatState& currentActor) {
    for (auto& s : currentActor.statuses) { 
        if (s->name == "Blessed") {
            currentActor.heal(s->intensity); 
        }
        
        if (s->name == "Regeneration") {
            currentActor.heal(s->intensity);
        }
    }

    for (auto& s : currentActor.statuses) {
        if (s->name == "Stun") {
            s->duration = 0;
        }
    }
}

/// @brief updates the statuses of the state
void CombatState::updateStatuses() {
    for (auto it = statuses.begin(); it != statuses.end(); ) {
        (*it)->onTurnStart(*this);
        if ((*it)->duration <= 0) it = statuses.erase(it);
        else ++it;
    }
}

/// @brief transforms the state
/// @param enemy the enemy of the state thats transforming
void CombatState::transform(CombatState& enemy) {
    isTransformed = true;
    transformTime += transformGain;

    if (onTransform) {
        onTransform->apply(*this, enemy, 0);
    }
}

/// @brief gives corruption to the state
/// @param corruptionGain the amount to gain
void CombatState::gainCorruption(int corruptionGain) {

    corruption.current += corruptionGain;
    if (corruption.current > corruption.max) {
        corruption.current = corruption.max;
    }
}

/// @brief gives mana to the state
/// @param manaGain the amount to gain
void CombatState::gainMana(int manaGain) {

    mana.current += manaGain;
    if (mana.current > mana.max) {
        mana.current = mana.max;
    }
}

/// @brief gets the intensity of the true void status amount
/// @return the intensity of the true void status
int CombatState::getTrueVoidMana() const {
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::TrueVoid) {
            return s->intensity;
        }
    }
    return 0;
}

/// @brief uses up true void mana
/// @param amount the amount of true void to use
void CombatState::consumeTrueVoid(int amount) {
    for (auto& s : statuses) {
        if (s && s->getType() == StatusType::TrueVoid) {
            s->intensity -= amount;
            return;
        }
    }
}

/// @brief gets the intensity of the zombie amry status
/// @return the intensity of the zombie army status
int CombatState::getZombieArmyIntensity() const {
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::ZombieArmy) {
            return s->intensity;
        }
    }
    return 0;
}

/// @brief gets the intensity of the skeleton amry status
/// @return the intensity of the skeleton army status
int CombatState::getSkeletonArmyIntensity() const {
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::SkeletonArmy) {
            return s->intensity;
        }
    }
    return 0;
}

/// @brief gets the intensity of the soul fragment status
/// @return the intensity of the soul fragment status
int CombatState::getSoulFragmentCount() const {
    int count = 0;
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::SoulFragment) {
            count += s->intensity;
        }
    }
    return count;
}

/// @brief adds a card to the hand of the state
/// @param cardKey the key (name) of the card to add
/// @param makeTemporary whether the card should be temporary (99% true)
void CombatState::addCardToHand(const std::string& cardKey, bool makeTemporary) {
    pendingCards.push_back({cardKey, makeTemporary});
}

/// @brief flushes the pending cards to the hand
void CombatState::flushPendingCards() {
    if (deck) {
        auto& hand = deck->getHand();
        std::vector<std::string> tempCards = {"Lead", "Gold", "Copper", "Iron", "Mercury", "Silver", "Tin", "Reaction", "Machine Power", "Spirit Slash"};

        for (const auto& pending : pendingCards) {
            Card card = CardFactory::create(pending.key);

            bool isTemp = std::find(tempCards.begin(), tempCards.end(), pending.key) != tempCards.end();

            if (isTemp || pending.forceTemporary) {
                card.isTemporary = true;
            }

            hand.push_back(std::move(card));
        }
        pendingCards.clear();
    }
}

/// @brief check wheter the status has a metal status
/// @param metal the metal status to check for
/// @return true if the state has the given metal status
bool CombatState::hasMetalStatus(StatusType metal) const {
    for (const auto& s : statuses) {
        if (s && s->getType() == metal) {
            return true;
        }
    }
    return false;
}

/// @brief gets the amount of metal statuses
/// @return the amount of metal statuses
int CombatState::getMetalStatusCount() const {
    int count = 0;
    for (const auto& s : statuses) {
        if (s) {
            StatusType type = s->getType();
            if (type == StatusType::Lead || type == StatusType::Gold ||
                type == StatusType::Copper || type == StatusType::Iron ||
                type == StatusType::Mercury || type == StatusType::Silver ||
                type == StatusType::Tin) {
                count++;
            }
        }
    }
    return count;
}

/// @brief removes the given metal status from the state
/// @param metal the metal to remove
void CombatState::removeMetalStatus(StatusType metal) {
    for (auto it = statuses.begin(); it != statuses.end(); ) {
        if (*it && (*it)->getType() == metal) {
            it = statuses.erase(it);
        } else {
            ++it;
        }
    }
}

/// @brief removes a random status from the state
void CombatState::removeRandomMetalStatus() {
    std::vector<StatusType> metals = {
        StatusType::Lead, StatusType::Gold, StatusType::Copper,
        StatusType::Iron, StatusType::Mercury, StatusType::Silver, StatusType::Tin
    };

    for (auto it = statuses.begin(); it != statuses.end(); ) {
        StatusType type = (*it)->getType();
        auto metalIt = std::find(metals.begin(), metals.end(), type);
        if (metalIt != metals.end()) {
            statuses.erase(it);
            return;
        } else {
            ++it;
        }
    }
}

/// @brief removes a random metal card from hand
/// @return the name of the removed card
std::string CombatState::removeRandomMetalCardFromHand() {
    if (!deck) return "";
 
    auto& hand = deck->getHand();
    std::vector<std::string> metalNames = {"Lead", "Gold", "Copper", "Iron", "Mercury", "Silver", "Tin"};

    for (auto it = hand.begin(); it != hand.end(); ++it) {
        auto nameIt = std::find(metalNames.begin(), metalNames.end(), it->name);
        if (nameIt != metalNames.end()) {
            std::string removedName = it->name;
            hand.erase(it);
            return removedName;
        }
    }
    return "";
}

/// @brief remobves all the machine power upgrades from the state
void CombatState::removeUpgradeStatus() {
    for (auto it = statuses.begin(); it != statuses.end(); ) {
        if (*it && (*it)->getType() == StatusType::MachineUpgrade) {
            it = statuses.erase(it);
        } else {
            ++it;
        }
    }
}
