#pragma once
#include "../Card/Card.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include <map>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

struct CardData {
    std::string name;
    int value;
    int cost;
    CardType type;
};

class CardFactory {
private:
    inline static std::map<std::string, CardData> CardRepo = {
        {"Fire Bolt",     {"Fire Bolt", 5, 1, CardType::Damage}},
        {"Mana Shield",   {"Mana Shield", 2, 1, CardType::Shield}},
        {"Slash",         {"Slash", 4, 1, CardType::Damage}},
        {"Defend",        {"Defend", 6, 1, CardType::Shield}},
        {"Holy Light",    {"Holy Light", 7, 1, CardType::Heal}},
        {"Smite",         {"Smite", 4, 1, CardType::Damage}},
        {"Phlox's Bloom", {"Phlox's Bloom", 10, 2, CardType::Damage}}
    };

public:
    static Card create(std::string key) {
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

    static std::vector<Card> getRandomLoot(int count) {
        std::vector<std::string> keys;
        for(auto const& [name, data] : CardRepo) keys.push_back(name);

        std::shuffle(keys.begin(), keys.end(), std::mt19937(std::random_device()()));
        
        std::vector<Card> loot;
        int actualCount = std::min((int)keys.size(), count);
        for(int i = 0; i < actualCount; ++i) {
            loot.push_back(create(keys[i]));
        }
        return loot;
    }
};
