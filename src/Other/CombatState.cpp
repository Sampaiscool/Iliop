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

int CombatState::takeDamage(int amount) {
    if (amount <= 0) return 0;
    int finalDamage = amount;

    for (auto& s : statuses) {
        if (s->name == "Death Mark") finalDamage = s->intensity * 2;
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
        if (absorbed > 0) damageEvents.emplace_back(absorbed, false);
    }

    int oldHP = hp.current;
    hp.current = std::max(0, hp.current - remainingDamage);
    int damageDealt = oldHP - hp.current;

    // add damage event
    if (damageDealt > 0) {
        damageEvents.emplace_back(damageDealt, false);
    }

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
    return damageDealt;
}

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

int CombatState::heal(int amount) {
    if (amount <= 0) return 0;
    int oldHP = hp.current;
    hp.current = std::min(hp.max, hp.current + amount);
    int healed = hp.current - oldHP;
    if (healed > 0) {
        damageEvents.emplace_back(healed, true);
    }
    return healed;
}

int CombatState::addShield(int amount) {
    if (amount <= 0) return 0;
    int oldShield = shield.current;
    shield.current = std::min(shield.max, shield.current + amount);
    int gained = shield.current - oldShield;
    if (gained > 0) {
        damageEvents.emplace_back(gained, false);
    }
    return gained;
}

void CombatState::draw(int drawAmount) {
    if (deck) {
        deck->drawCard(drawAmount);
    }
}

void CombatState::applyStatus(std::unique_ptr<Status> newStatus) {
    if (!newStatus) return;

    // suprime machine increment
    for (auto& s : statuses) {
        if (s->getType() == StatusType::SupremeMachine) {
            s->intensity += 1;
            break;
        }
    }
 
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

void CombatState::gainCorruption(int corruptionGain) {

    corruption.current += corruptionGain;
    if (corruption.current > corruption.max) {
        corruption.current = corruption.max;
    }
}

void CombatState::gainMana(int manaGain) {

    mana.current += manaGain;
    if (mana.current > mana.max) {
        mana.current = mana.max;
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

int CombatState::getZombieArmyIntensity() const {
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::ZombieArmy) {
            return s->intensity;
        }
    }
    return 0;
}

int CombatState::getSkeletonArmyIntensity() const {
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::SkeletonArmy) {
            return s->intensity;
        }
    }
    return 0;
}

int CombatState::getSoulFragmentCount() const {
    int count = 0;
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::SoulFragment) {
            count += s->intensity;
        }
    }
    return count;
}

int CombatState::getCatalystIntensity() const {
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::Catalyst) {
            return s->intensity;
        }
    }
    return 0;
}

int CombatState::getPotionCount() const {
    int count = 0;
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::Potion) {
            count += s->intensity;
        }
    }
    return count;
}

int CombatState::getElixirCount() const {
    int count = 0;
    for (const auto& s : statuses) {
        if (s && s->getType() == StatusType::Elixir) {
            count += s->intensity;
        }
    }
    return count;
}

int CombatState::getModifiedHeal(int baseHeal) const {
    int catalyst = getCatalystIntensity();
    return baseHeal + (catalyst * 2);
}

int CombatState::getModifiedShield(int baseShield) const {
    int catalyst = getCatalystIntensity();
    return baseShield + (catalyst * 2);
}

int CombatState::getManaCostReduction() const {
    int elixir = getElixirCount();
    return std::min(elixir, 3);
}

void CombatState::addCardToHand(const std::string& cardKey, bool makeTemporary) {
    pendingCards.push_back({cardKey, makeTemporary});
}

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

bool CombatState::hasMetalStatus(StatusType metal) const {
    for (const auto& s : statuses) {
        if (s && s->getType() == metal) {
            return true;
        }
    }
    return false;
}

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

void CombatState::removeMetalStatus(StatusType metal) {
    for (auto it = statuses.begin(); it != statuses.end(); ) {
        if (*it && (*it)->getType() == metal) {
            it = statuses.erase(it);
        } else {
            ++it;
        }
    }
}

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

void CombatState::removeUpgradeStatus() {
    for (auto it = statuses.begin(); it != statuses.end(); ) {
        if (*it && (*it)->getType() == StatusType::MachineUpgrade) {
            it = statuses.erase(it);
        } else {
            ++it;
        }
    }
}
