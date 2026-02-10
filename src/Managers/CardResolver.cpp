#include "CardResolver.h"
#include <algorithm>

void CardResolver::play(const Card& card, CombatState& player, CombatState& enemy) {
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
}
