#include "CardResolver.h"
#include <algorithm>

bool CardResolver::play(
    const Card& card,
    CombatState& player,
    CombatState& enemy,
    Deck& deck)
{
    int trueVoidMana = player.getTrueVoidMana();
    int totalAvailable = player.mana.current + trueVoidMana;

    if (totalAvailable < card.cost)
        return false;

    // pay for card
    int remainingCost = card.cost;
    if (trueVoidMana > 0) {
        int usedFromVoid = std::min(trueVoidMana, remainingCost);
        player.consumeTrueVoid(usedFromVoid);
        remainingCost -= usedFromVoid;
    }
    player.mana.current -= remainingCost;

    // check if current corruption amount is enough for a transformation
    bool isCorrupted = (player.corruption.current >= player.transformThreshold);

    // trigger card behavior
    card.play(player, enemy, isCorrupted);

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


    for (auto it = enemy.statuses.begin(); it != enemy.statuses.end(); ) {
        if ((*it)->getType() == StatusType::VoidMark) {
            (*it)->intensity++;

            if ((*it)->intensity >= 5) {
                enemy.takeDamage(5);                                        // Big damage
                player.applyStatus(std::make_unique<TrueVoidStatus>(2, 5)); // Big Mana
                deck.drawCard(5);                                           // Big Hand refill

                it = enemy.statuses.erase(it);
                continue; 
            }
        }
        ++it;
    }

    // global rules
    player.corruption.current =
        std::min(player.corruption.current + 1,
                 player.corruption.max);

    return true;
}
