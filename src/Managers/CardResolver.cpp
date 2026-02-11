#include "CardResolver.h"
#include <algorithm>

bool CardResolver::play(const Card& card, CombatState& player, CombatState& enemy) {
    if (player.mana.current >= card.cost) {
        switch (card.effect) {
            case CardEffect::DealDamage:
                enemy.hp.current -= card.value;
                break;
            case CardEffect::Heal:
                player.hp.current = std::min(player.hp.current + card.value, player.hp.max);
                break;
            case CardEffect::GainShield:
                player.shield.current += card.value;
                break;
        }
        player.mana.current = player.mana.current - card.cost;
        player.corruption.current = std::min(player.corruption.current + 1, player.corruption.max);

        return true;
    }
    else {
      return false;
    }
}
