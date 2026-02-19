#pragma once
#include "../Character/Character.h"
#include "../Managers/CardFactory.h"
#include "../Effects/MultiEffect.h"

class CharacterFactory {
public:
    static Character create(Class classType, CharacterName nameType) {
        std::vector<Card> deck;
        CombatState stats;
        int transformCorruption = 0, transformTime = 0;
        std::string displayName = "";

        // create the decks and classes:
        // character: hp - shield - mana - corruption
        switch (classType) {
            case Class::Mage:
                stats = {{15, 15}, {0, 10}, {4, 4}, {0, 3}};
                transformCorruption = 3; transformTime = 2;
                for (int i = 0; i < 6; ++i) deck.push_back(CardFactory::create("Fire Bolt"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Primal Arrow"));
                break;
            case Class::Warrior:
                stats = {{40, 40}, {0, 20}, {2, 2}, {0, 1}};
                transformCorruption = 1; transformTime = 2;
                for (int i = 0; i < 8; ++i) deck.push_back(CardFactory::create("Slash"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Defend"));
                break;
            case Class::Cleric:
                stats = {{30, 30}, {0, 10}, {3, 3}, {0, 2}};
                transformCorruption = 2; transformTime = 3;
                for (int i = 0; i < 6; ++i) deck.push_back(CardFactory::create("Holy Light"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Smite"));
                break;
        }

        // create the identity of the characters
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
        auto keys = CardFactory::getAllAvailableKeys();
        std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device()()));
        if (keys.size() > count) keys.resize(count);
        return keys;
    }
};
