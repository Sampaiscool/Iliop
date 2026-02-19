#pragma once
#include "../Card/Card.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include "../Effects/EffectTypes/Ranger/PrimalArrow.h"

#include <map>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

struct CardData {
    std::string name;
    std::string description;
    int value;
    int corruptedValue;
    int cost;
    CardType type;
    CardTheme theme;
};

class CardFactory {
private:
    inline static std::map<std::string, CardData> CardRepo = {
        // Format: { Name, Description, Value, CorruptedValue, Cost, Type, Theme }
        {"Fire Bolt",       {"Fire Bolt", "Deals fire damage.", 5, 2, 1, CardType::Damage, CardTheme::Red}},
        {"Mana Shield",     {"Mana Shield", "Gain magical protection.", 2, 3, 1, CardType::Shield, CardTheme::Blue}},
        {"Slash",           {"Slash", "A basic melee attack.", 4, 3, 1, CardType::Damage, CardTheme::Red}},
        {"Defend",          {"Defend", "Raise your guard.", 6, 4, 1, CardType::Shield, CardTheme::Blue}},
        {"Holy Light",      {"Holy Light", "Restore health.", 7, 3, 1, CardType::Heal, CardTheme::Gold}},
        {"Smite",           {"Smite", "Strike with holy power.", 1, 7, 1, CardType::Damage, CardTheme::Gold}},
        {"Phlox's Bloom",   {"Phlox's Bloom", "A powerful floral burst.", 10, 10, 2, CardType::Damage, CardTheme::Purple}},
        {"Primal Arrow",    {"Primal Arrow", "Transform or Slash 5x and Bleed.", 2, 2, 1, CardType::PrimalArrow, CardTheme::Green}}
    };

public:
    static Card create(std::string key) {
        if (CardRepo.find(key) == CardRepo.end()) return Card();

        CardData data = CardRepo[key];
        Card card;
        card.name = data.name;
        card.description = data.description;
        card.cost = data.cost;
        card.value = data.value;
        card.corruptedValue = data.corruptedValue;
        card.type = data.type;
        card.theme = data.theme;

        switch (card.type) {
            case CardType::Damage:        card.effect = std::make_unique<DamageEffect>();         break;
            case CardType::Heal:          card.effect = std::make_unique<HealEffect>();           break;
            case CardType::Shield:        card.effect = std::make_unique<ShieldEffect>();         break;
            case CardType::PrimalArrow:   card.effect = std::make_unique<PrimalArrowEffect>();    break;
        }
        return card;
    }

    static std::vector<std::string> getAllAvailableKeys() {
        std::vector<std::string> keys;
        for(auto const& [name, data] : CardRepo) keys.push_back(name);
        return keys;
    }

    static std::vector<Card> getRandomLoot(int count) {
        auto keys = getAllAvailableKeys();
        std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device()()));

        std::vector<Card> loot;
        int actualCount = std::min((int)keys.size(), count);
        for(int i = 0; i < actualCount; ++i) {
            loot.push_back(create(keys[i]));
        }
        return loot;
    }

    sf::Color getThemeColor(CardTheme theme) {
        switch(theme) {
            case CardTheme::Red:    return sf::Color(200, 50, 50);
            case CardTheme::Blue:   return sf::Color(50, 50, 200);
            case CardTheme::Green:  return sf::Color(50, 180, 50);
            case CardTheme::Purple: return sf::Color(140, 50, 200);
            // ... add the rest
            default: return sf::Color::White;
        }
    }
};
