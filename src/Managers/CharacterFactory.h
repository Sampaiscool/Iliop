#pragma once
#include "../Character/Character.h"
#include "../Managers/CardFactory.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include "../Effects/MultiEffect.h"
#include <string>
#include <vector>
#include <random>
#include <map>

class CharacterFactory {
private:
    // static map for centrilized cards
    inline static std::map<std::string, CardData> CardRepo = {
        {"Fire Bolt",     {"Fire Bolt", 5, 1, CardType::Damage}},
        {"Mana Shield",   {"Mana Shield", 2, 1, CardType::Shield}},
        {"Slash",         {"Slash", 4, 1, CardType::Damage}},
        {"Defend",        {"Defend", 6, 1, CardType::Shield}},
        {"Holy Light",    {"Holy Light", 7, 1, CardType::Heal}},
        {"Smite",         {"Smite", 4, 1, CardType::Damage}},
        {"Phlox's Bloom", {"Phlox's Bloom", 10, 2, CardType::Damage}}
    };

    static Card makeCard(std::string key) {
        CardData data = CardRepo[key];
        Card card;
        card.name = data.name;
        card.cost = data.cost;
        card.value = data.value;
        card.type = data.type;

        switch (card.type) {
            case CardType::Damage: card.effect = std::make_unique<DamageEffect>(card.value); break;
            case CardType::Heal:   card.effect = std::make_unique<HealEffect>(card.value);   break;
            case CardType::Shield: card.effect = std::make_unique<ShieldEffect>(card.value); break;
        }
        return card;
    }

public:
    static Character create(Class classType, CharacterName nameType) {
        std::vector<Card> deck;
        CombatState stats;
        int transformCorruption = 0, transformTime = 0;
        std::string displayName = "";

        //Class stats: hp - shield - mana - corruption
        switch (classType) {
            case Class::Mage:
                stats = {{15, 15}, {0, 10}, {4, 4}, {0, 10}};
                transformCorruption = 3; transformTime = 2;
                for (int i = 0; i < 6; ++i) deck.push_back(CardFactory::create("Fire Bolt"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Mana Shield"));
                break;
            case Class::Warrior:
                stats = {{40, 40}, {0, 20}, {2, 2}, {0, 10}};
                transformCorruption = 1; transformTime = 2;
                for (int i = 0; i < 8; ++i) deck.push_back(CardFactory::create("Slash"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Defend"));
                break;
            case Class::Cleric:
                stats = {{30, 30}, {0, 10}, {3, 3}, {0, 10}};
                transformCorruption = 2; transformTime = 3;
                for (int i = 0; i < 6; ++i) deck.push_back(CardFactory::create("Holy Light"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Smite"));
                break;
        }

        switch (nameType) {
            case CharacterName::Hiroshi:
                displayName = "Hiroshi";
                stats.shield.max += 5;
                stats.transformationProc = std::make_unique<DamageEffect>(4);
                break;
            case CharacterName::Phlox: {
                displayName = "Phlox";
                deck.push_back(CardFactory::create("Phlox's Bloom"));
                auto multi = std::make_unique<MultiEffect>();
                multi->add(std::make_unique<ShieldEffect>(3));
                multi->add(std::make_unique<HealEffect>(2));
                stats.transformationProc = std::move(multi);
                break;
                }
            case CharacterName::MightyFire:
                displayName = "Mighty Fire";
                stats.mana.max += 1; stats.mana.current += 1;
                stats.transformationProc = std::make_unique<DamageEffect>(1);
                break;
        }

        return Character(classType, nameType, displayName, std::move(deck), std::move(stats), transformCorruption, transformTime);
    }

    static std::vector<std::string> getRandomLootOptions(int count) {
        std::vector<std::string> keys;
        for(auto const& [name, data] : CardRepo) {
            keys.push_back(name);
        }
        std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device()()));

        if (keys.size() > count) keys.resize(count);

        return keys;
    }

    // And a way to turn a name back into a real Card for the deck
    static Card createCardByName(std::string name) {
        return CardFactory::create(name);
    }
};
