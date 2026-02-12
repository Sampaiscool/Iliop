#pragma once
#include "../Character/Character.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include <string>

class CharacterFactory {
public:
    static Character create(CharacterClass type) {
    std::vector<Card> deck;
    CombatState stats;
    int transformCorruption = 0;

    /*
    character stats:     current - max
                         HP - shield - mana - corruption
    card stats :         value - cost
    */

    switch (type) {
    case CharacterClass::Mage:
        // Mages: Low HP, High Mana
        stats = {{15, 15}, {0, 10}, {4, 4}, {0, 10}};
        transformCorruption = 3;
        for (int i = 0; i < 6; ++i) deck.push_back(makeDamageCard("Fire Bolt", 5, 1));
        for (int i = 0; i < 4; ++i) deck.push_back(makeShieldCard("Mana Shield", 2, 1));
        break;

    case CharacterClass::Warrior:
        // Warriors: High HP, High Shield capacity, Low Mana
        stats = {{40, 40}, {0, 20}, {2, 2}, {0, 10}};
        transformCorruption = 2;
        for (int i = 0; i < 8; ++i) deck.push_back(makeDamageCard("Slash", 4, 1));
        for (int i = 0; i < 4; ++i) deck.push_back(makeShieldCard("Defend", 6, 1));
        break;

    case CharacterClass::Cleric:
        // Clerics: Balanced, Higher Corruption resistance?
        stats = {{30, 30}, {0, 10}, {3, 3}, {0, 10}};
        transformCorruption = 2;
        for (int i = 0; i < 6; ++i) deck.push_back(makeHealCard("Holy Light", 7, 1));
        for (int i = 0; i < 4; ++i) deck.push_back(makeDamageCard("Smite", 4, 1));
        break;
    }

    return Character(type, std::move(deck), stats, transformCorruption);
}
private:
    static Card makeDamageCard(std::string name, int value, int cost) {
        Card card;
        card.name = name;
        card.type = CardType::Damage;
        card.effect = std::make_unique<DamageEffect>(value);
        card.cost = cost;
        return card;
    }

    static Card makeHealCard(std::string name, int value, int cost) {
        Card card;
        card.name = name;
        card.type = CardType::Heal;
        card.effect = std::make_unique<HealEffect>(value);
        card.cost = cost;
        return card;
    }

    static Card makeShieldCard(std::string name, int value, int cost) {
        Card card;
        card.name = name;
        card.type = CardType::Shield;
        card.effect = std::make_unique<ShieldEffect>(value);
        card.cost = cost;
        return card;
    }
};
