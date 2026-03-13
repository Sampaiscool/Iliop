#include "CardResolver.h"
#include <algorithm>

bool CardResolver::play(
    const Card& card,
    CombatState& player,
    CombatState& enemy,
    Deck& deck)
{
    player.deck = &deck;

    int trueVoidMana = player.getTrueVoidMana();
    int totalAvailable = player.mana.current + trueVoidMana;

    int effectiveCost = std::max(0, card.cost - card.costReduction);
    
    if (card.freeOnceCount > 0) {
        effectiveCost = 0;
    }

    if (totalAvailable < effectiveCost)
        return false;

    // pay for card
    int remainingCost = effectiveCost;

    // first normal mana
    int usedFromMana = std::min(player.mana.current, remainingCost);
    player.mana.current -= usedFromMana;
    remainingCost -= usedFromMana;

    // whatever is left gets paid by true void
    if (remainingCost > 0) {
        int usedFromVoid = std::min(trueVoidMana, remainingCost);
        player.consumeTrueVoid(usedFromVoid);
        remainingCost -= usedFromVoid;
    }

    // check if current corruption amount is enough for a transformation
    bool isCorrupted = (player.corruption.current >= player.transformThreshold);

    // trigger card behavior
    card.play(player, enemy, isCorrupted);
    
    // handle replay (stackable - play multiple times)
    for (int i = 0; i < card.replayCount; i++) {
        card.play(player, enemy, isCorrupted);
    }
    
    // use up freeOnce (stackable - each use reduces the count)
    if (card.freeOnceCount > 0) {
        const_cast<Card&>(card).freeOnceCount--;
    }

    // flush any pending cards
    player.flushPendingCards();

    if (player.isTransformed && player.onCardPlayProc) {
        player.onCardPlayProc->apply(player, enemy, 0);
    }

    if (isCorrupted) {
        for (auto& status : player.statuses) {
            if (status->getType() == StatusType::CorruptedVoid) {
                deck.drawCard(status->intensity);
            }
        }
    }

    if (player.hp.current < player.hp.max) {
        for (auto& status : player.statuses) {
            if (status->getType() == StatusType::BloodLust) {
                deck.drawCard(status->intensity);
            }
        }
    }

    // void maark increment
    for (auto it = enemy.statuses.begin(); it != enemy.statuses.end(); ) {
        if ((*it)->getType() == StatusType::VoidMark) {
            (*it)->intensity++;

            if ((*it)->intensity >= 5) {
                enemy.takeDamage(5);
                player.applyStatus(std::make_unique<TrueVoidStatus>(2, 5));
                deck.drawCard(5);

                it = enemy.statuses.erase(it);
                continue; 
            }
        }
        ++it;
    }

    // weaker creator increment
    for (auto& status : enemy.statuses) {
        if (status->getType() == StatusType::WeakerCreator) {
            int increment = isCorrupted ? 2 : 1;
            status->intensity += increment;

            // at 7, transform and give Suprime Machine
            if (status->intensity >= 7) {
                player.transform(enemy);
                bool hasSuprimeMachine = false;
                for (const auto& s : player.statuses) {
                    if (s && s->getType() == StatusType::SupremeMachine) {
                        hasSuprimeMachine = true;
                        break;
                    }
                }
                if (!hasSuprimeMachine) {
                    player.applyStatus(std::make_unique<SuprimeMachineStatus>(3, 0));
                }
                status->intensity = 0;
            }
            break;
        }
    }

    // suprime machine increment
    for (auto& status : player.statuses) {
        if (status->getType() == StatusType::SupremeMachine) {
            if (status->intensity >= 4) {
                player.applyStatus(std::make_unique<TrueVoidStatus>(2, 3));
                deck.drawCard(3);
                status->intensity = 0;
            }
            break;
        }
    }

    // holy spirit + forbidden droplet
    // NOTE: collect intensities first to avoid iterator invalidation when applying new statuses
    int holySpiritIntensity = 0;
    bool hasForbiddenDroplet = false;
    for (auto& status : player.statuses) {
        if (status->getType() == StatusType::HolySpirit) {
            holySpiritIntensity = status->intensity;
        }
        if (status->getType() == StatusType::ForbiddenDroplet) {
            hasForbiddenDroplet = true;
        }
    }
    if (holySpiritIntensity > 0) {
        player.applyStatus(std::make_unique<BlessedStatus>(1, holySpiritIntensity));
        enemy.applyStatus(std::make_unique<BlessedStatus>(1, holySpiritIntensity));
    }
    if (hasForbiddenDroplet) {
        if (player.corruption.current == player.corruption.max) {
            player.transform(enemy);
        }
    }

    bool hasFlight = false;
    int flightIntensity = 0;

    for (auto& status : player.statuses) {
        if (status->getType() == StatusType::Flight) {
            hasFlight = true;
            flightIntensity = status->intensity;
        }
    }

    int corruptionGain = (hasFlight) ? (1 + flightIntensity) : 1;

    // global rules
    player.gainCorruption(corruptionGain);

    return true;
}
