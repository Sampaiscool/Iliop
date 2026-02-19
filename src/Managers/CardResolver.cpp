#include "CardResolver.h"
#include <algorithm>

bool CardResolver::play(
    const Card& card,
    CombatState& player,
    CombatState& enemy)
{
    if (player.mana.current < card.cost)
        return false;

    // pay cost
    player.mana.current -= card.cost;

    // check if current corruption amount is enough for a transformation
    bool isCorrupted = (player.corruption.current >= player.transformThreshold);

    // trigger card behavior
    card.play(player, enemy, isCorrupted);

    if (player.isTransformed && player.transformationProc) {
        player.transformationProc->apply(player, enemy, 0);
    }

    // global rules
    player.corruption.current =
        std::min(player.corruption.current + 1,
                 player.corruption.max);

    return true;
}
