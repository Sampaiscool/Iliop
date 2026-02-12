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

    // trigger card behavior
    card.play(player, enemy);

    // global rules
    player.corruption.current =
        std::min(player.corruption.current + 1,
                 player.corruption.max);

    return true;
}
