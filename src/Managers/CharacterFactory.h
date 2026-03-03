#pragma once
#include "../Character/Character.h"
#include "../Managers/CardFactory.h"
#include "../Effects/MultiEffect.h"
#include "../Effects/EffectTypes/Ranger/PredatorInstinct.h"
#include "../Other/AllStatuses.h"

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
                stats = CombatState{{15, 15}, {0, 10}, {4, 4}, {0, 3}};
                transformCorruption = 3; transformTime = 2;
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Unstable Volley"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Void Grasp"));
                for (int i = 0; i < 2; ++i) deck.push_back(CardFactory::create("Eldritch Blast"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Astral Shift"));
                break;
            case Class::Warrior:
                stats = CombatState{{40, 40}, {0, 20}, {2, 2}, {0, 1}};
                transformCorruption = 1; transformTime = 2;
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Iron Will"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Shatter Spleen"));
                for (int i = 0; i < 3; ++i) deck.push_back(CardFactory::create("Aegis Strike"));
                for (int i = 0; i < 2; ++i) deck.push_back(CardFactory::create("Warrior Pact"));
                for (int i = 0; i < 2; ++i) deck.push_back(CardFactory::create("Shield Bash"));
                break;
            case Class::Cleric:
                stats = CombatState{{30, 30}, {0, 10}, {3, 3}, {0, 2}};
                transformCorruption = 2; transformTime = 2;
                for (int i = 0; i < 5; ++i) deck.push_back(CardFactory::create("Holy Light"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Purge"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Ritual"));
                for (int i = 0; i < 5; ++i) deck.push_back(CardFactory::create("Condemn"));
                for (int i = 0; i < 2; ++i) deck.push_back(CardFactory::create("Blinding Light"));
                break;
            case Class::Ranger:
                stats = CombatState{{25, 25}, {0, 10}, {3, 3}, {0, 2}};
                transformCorruption = 2; transformTime = 2;
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Primal Arrow"));
                for (int i = 0; i < 4; ++i) deck.push_back(CardFactory::create("Aim"));
                for (int i = 0; i < 3; ++i) deck.push_back(CardFactory::create("Arrow Volley"));
                for (int i = 0; i < 3; ++i) deck.push_back(CardFactory::create("Jump"));
                for (int i = 0; i < 2; ++i) deck.push_back(CardFactory::create("Metamorphosis"));
                break;
        }

        // create the identity of the characters
        switch (nameType) {
            case CharacterName::Hiroshi:
                displayName = "Hiroshi";
                stats.shield.max += 10;
                stats.onTransform = std::make_unique<ShieldEffect>(10);
                stats.onCardPlayProc = std::make_unique<RagingDefence>(3);
                stats.passiveValue = 2;
                break;
            case CharacterName::Phlox: {
                displayName = "Phlox";
                deck.push_back(CardFactory::create("Phlox's Bloom"));
                auto multi = std::make_unique<MultiEffect>();
                multi->add(std::make_unique<HolyZeal>(1));
                multi->add(std::make_unique<HealEffect>(1));
                stats.onCardPlayProc = std::move(multi);
                break;
            }
            case CharacterName::Vortex: {
                displayName = "Vortex";
                stats.mana.max += 1; stats.mana.current += 1;
                stats.onTransform = std::make_unique<VoidProwess>(3);
                auto multi = std::make_unique<MultiEffect>();
                multi->add(std::make_unique<DamageEffect>(1));
                multi->add(std::make_unique<Infinity>(1));
                stats.onCardPlayProc = std::move(multi);
                stats.passiveValue = 1;
                break;
            }
            case CharacterName::MathewsLift: {
                displayName = "Mathews Lift";
                stats.hp.max -= 5; stats.hp.current = stats.hp.max;
                stats.transformThreshold += 2;
                stats.onTransform = std::make_unique<PredatorInstinct>(3);
                auto multi = std::make_unique<MultiEffect>();
                auto prowl = std::make_unique<PredatorInstinct>(1);
                multi->add(std::move(prowl));
                multi->add(std::make_unique<HealEffect>(1));
                stats.onCardPlayProc = std::move(multi);
                stats.passiveValue = 1;
                // THE COOL PART: Apply Blood Lust when wounded - gives card draw!
                stats.statuses.push_back(std::make_unique<BloodLustStatus>(99, 1));
                break;
            }
        }

        return Character(classType, nameType, displayName, std::move(deck), std::move(stats), transformCorruption, transformTime);
    }

    static std::vector<std::string> getRandomLootOptions(int count) {
        auto keys = CardFactory::getAllAvailableKeys();
        
        // Filter out fusion cards from loot!
        keys.erase(std::remove_if(keys.begin(), keys.end(), [](const std::string& key) {
            return key == "Divine Arrow" || key == "Void Storm" || 
                   key == "Beast Rampage" || key == "Cosmic Shield" || 
                   key == "Blood Frenzy" || key == "Omega Annihilation" ||
                   key == "Universal Singularity" || key == "Primordial Chaos" ||
                   key == "Existential Crisis";
        }), keys.end());
        
        std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device()()));
        if (keys.size() > count) keys.resize(count);
        return keys;
    }
};
