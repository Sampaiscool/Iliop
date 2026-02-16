#pragma once
#include "../Character/Character.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include "../Effects/MultiEffect.h"
#include <string>
#include <vector>

class CharacterFactory {
public:
    static Character create(Class classType, CharacterName nameType) {
        std::vector<Card> deck;
        CombatState stats;
        int transformCorruption = 0;
        int transformTime = 0;
        std::string displayName = "";
        
        /*
        class:  HP - shield - mana - corruptionThreshold

        card:   name - value - cost
        */

        // setup correct class
        switch (classType) {
            case Class::Mage:
                stats = {{15, 15}, {0, 10}, {4, 4}, {0, 10}};
                transformCorruption = 3;
                transformTime = 2;
                for (int i = 0; i < 6; ++i) deck.push_back(makeDamageCard("Fire Bolt", 5, 1));
                for (int i = 0; i < 4; ++i) deck.push_back(makeShieldCard("Mana Shield", 2, 1));
                break;
            case Class::Warrior:
                stats = {{40, 40}, {0, 20}, {2, 2}, {0, 10}};
                transformCorruption = 1;
                transformTime = 2;
                for (int i = 0; i < 8; ++i) deck.push_back(makeDamageCard("Slash", 4, 1));
                for (int i = 0; i < 4; ++i) deck.push_back(makeShieldCard("Defend", 6, 1));
                break;
            case Class::Cleric:
                stats = {{30, 30}, {0, 10}, {3, 3}, {0, 10}};
                transformCorruption = 2;
                transformTime = 3;
                for (int i = 0; i < 6; ++i) deck.push_back(makeHealCard("Holy Light", 7, 1));
                for (int i = 0; i < 4; ++i) deck.push_back(makeDamageCard("Smite", 4, 1));
                break;
        }

        // setup identity
        switch (nameType) {
            case CharacterName::Hiroshi:
                displayName = "Hiroshi";
                stats.shield.max += 5;
                stats.transformationProc = std::make_unique<DamageEffect>(4);
                break;

            case CharacterName::Phlox: { // Added braces for variable scope
                displayName = "Phlox";
                deck.push_back(makeDamageCard("Phlox's Bloom", 10, 2));
                auto multi = std::make_unique<MultiEffect>();
                multi->add(std::make_unique<ShieldEffect>(3));
                multi->add(std::make_unique<HealEffect>(2));
                stats.transformationProc = std::move(multi);
                break;
            }

            case CharacterName::MightyFire:
                displayName = "Mighty Fire";
                stats.mana.max += 1;
                stats.mana.current += 1;
                stats.transformationProc = std::make_unique<DamageEffect>(1);
                break;
        }

        // give everything to the constructor
        return Character(classType, nameType, displayName, std::move(deck), std::move(stats), transformCorruption, transformTime);
    }

private:
    static Card makeDamageCard(std::string name, int value, int cost) {
        Card card;
        card.name = name;
        card.type = CardType::Damage;
        card.effect = std::make_unique<DamageEffect>(value);
        card.cost = cost;
        card.value = value;
        return card;
    }

    static Card makeHealCard(std::string name, int value, int cost) {
        Card card;
        card.name = name;
        card.type = CardType::Heal;
        card.effect = std::make_unique<HealEffect>(value);
        card.cost = cost;
        card.value = value;
        return card;
    }

    static Card makeShieldCard(std::string name, int value, int cost) {
        Card card;
        card.name = name;
        card.type = CardType::Shield;
        card.effect = std::make_unique<ShieldEffect>(value);
        card.cost = cost;
        card.value = value;
        return card;
    }
};
