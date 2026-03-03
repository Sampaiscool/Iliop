#pragma once
#include "../Card/Card.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include "../Effects/FusionEffect.h"

// Fusion effects:
#include "../Effects/EffectTypes/Fusion/DivineArrowFusion.h"
#include "../Effects/EffectTypes/Fusion/VoidStormFusion.h"
#include "../Effects/EffectTypes/Fusion/BeastRampageFusion.h"
#include "../Effects/EffectTypes/Fusion/CosmicShieldFusion.h"
#include "../Effects/EffectTypes/Fusion/BloodFrenzyFusion.h"
#include "../Effects/EffectTypes/Fusion/OmegaAnnihilation.h"
#include "../Effects/EffectTypes/Fusion/UniversalSingularity.h"
#include "../Effects/EffectTypes/Fusion/PrimordialChaos.h"
#include "../Effects/EffectTypes/Fusion/ExistentialCrisis.h"

// ranger:
#include "../Effects/EffectTypes/Ranger/PrimalArrow.h"
#include "../Effects/EffectTypes/Ranger/Aim.h"
#include "../Effects/EffectTypes/Ranger/ArrowVolley.h"
#include "../Effects/EffectTypes/Ranger/Jump.h"
#include "../Effects/EffectTypes/Ranger/Metamorphosis.h"

// mage:
#include "../Effects/EffectTypes/Mage/AstralShift.h"
#include "../Effects/EffectTypes/Mage/EldritchBlast.h"
#include "../Effects/EffectTypes/Mage/UnstableVolley.h"
#include "../Effects/EffectTypes/Mage/VoidGrasp.h"
#include "../Effects/EffectTypes/Mage/VoidProwess.h"
#include "../Effects/EffectTypes/Mage/Infinity.h"

// warrior:
#include "../Effects/EffectTypes/Warrior/AegisStrike.h"
#include "../Effects/EffectTypes/Warrior/IronWill.h"
#include "../Effects/EffectTypes/Warrior/ShatterSpleen.h"
#include "../Effects/EffectTypes/Warrior/ShieldBash.h"
#include "../Effects/EffectTypes/Warrior/WarriorPact.h"
#include "../Effects/EffectTypes/Warrior/RagingDefence.h"

// cleric:
#include "../Effects/EffectTypes/Cleric/BlindingLight.h"
#include "../Effects/EffectTypes/Cleric/Condemn.h"
#include "../Effects/EffectTypes/Cleric/HolyZeal.h"
#include "../Effects/EffectTypes/Cleric/Purge.h"
#include "../Effects/EffectTypes/Cleric/Ritual.h"

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
        // format: { name, description, value, corruptedValue, cost, type, theme }
        {"Fire Bolt",       {"Fire Bolt", "Deals fire damage.", 5, 2, 1, CardType::Damage, CardTheme::Red}},
        {"Mana Shield",     {"Mana Shield", "Gain magical protection.", 2, 3, 1, CardType::Shield, CardTheme::Gray}},
        {"Slash",           {"Slash", "A basic melee attack.", 4, 3, 1, CardType::Damage, CardTheme::Red}},
        {"Defend",          {"Defend", "Raise your guard.", 6, 4, 1, CardType::Shield, CardTheme::Gray}},
        {"Smite",           {"Smite", "Strike with holy power.", 1, 7, 1, CardType::Damage, CardTheme::Red}},
        {"Phlox's Bloom",   {"Phlox's Bloom", "A powerful floral burst.", 10, 10, 2, CardType::Damage, CardTheme::Red}},

        // ranger
        {"Primal Arrow",    {"Primal Arrow", "Deal damage and transform\n(deal damage 5 times (can bleed))", 1, 1, 1, CardType::PrimalArrow, CardTheme::Red}},
        {"Aim",             {"Aim", "Gain Locked and heal\n(Instead gain Raging Bear and inflict bleed)", 2, 1, 1, CardType::Aim, CardTheme::Gray}},
        {"Arrow Volley",    {"Arrow Volley", "Fire a volley of arrows.\nTransformed: More hits + chance to bleed", 2, 1, 1, CardType::ArrowVolley, CardTheme::Red}},
        {"Jump",            {"Jump", "Gain shield and Defence Up.\nTransformed: Deal damage + gain Defence Up", 4, 3, 1, CardType::Jump, CardTheme::Teal}},
        {"Metamorphosis",  {"Metamorphosis", "Transform into beast form.\nTransformed: Extend transform + heal", 1, 2, 1, CardType::Metamorphosis, CardTheme::Green}},

        // mage
        {"Unstable Volley", {"Unstable Volley", "Deal damage + gain 2 corruption, \nThen if you are at max: transform\n(Gain Overload)", 2, 4, 1, CardType::UnstableVolley, CardTheme::Purple}},
        {"Eldritch Blast",  {"Eldritch Blast", "Deal damage, scales with corruption.\nThen gain 2 corruption", 4, 2, 2, CardType::EldritchBlast, CardTheme::Purple}},
        {"Void Grasp",      {"Void Grasp", "Deal damage and Apply Void Mark.\nStronger if transformed.\nAlso shreds shields", 1, 1, 1, CardType::VoidGrasp, CardTheme::Red}},
        {"Astral Shift",    {"Astral Shift", "Transformed: transform\nElse: Gain shield + Lose all corruption", 5, 5, 1, CardType::AstralShift, CardTheme::Gray}},
        {"Void Prowess",    {"Void Prowess", "Gain true void", 2, 2, 2, CardType::VoidProwess, CardTheme::Blue}},

        // warrior
        {"Aegis Strike",    {"Aegis Strike", "Deal damage to yourself and enemy.\nTransform after", 6, 8, 1, CardType::AegisStrike, CardTheme::Red}},
        {"Iron Will",       {"Iron Will", "Deal some damage to yourself,\nGain a shield", 10, 15, 1, CardType::IronWill, CardTheme::Gray}},
        {"Shatter Spleen",  {"Shatter Spleen", "Gain Defense Up based on Shield.\nDeals damage if you have max shield\n(Give weaken)", 5, 8, 2, CardType::ShatterSpleen, CardTheme::Red}},
        {"Shield Bash",     {"Shield Bash", "Consume Max Shield to Stun\nAlso deals damage.", 10, 20, 2, CardType::ShieldBash, CardTheme::Red}},
        {"Warrior Pact",    {"Warrior Pact", "Sacrifice HP for 2 Mana.", 5, 0, 0, CardType::WarriorPact, CardTheme::Purple}},

        // cleric
        {"Holy Light",      {"Holy Light", "Restore health.", 7, 3, 1, CardType::Heal, CardTheme::Green}},
        {"Blinding Light",  {"Blinding Light", "Deal damage and apply Damage Down.\nIf target has 2+ statuses:\nstun and damage becomes increased.", 4, 6, 1, CardType::BlindingLight, CardTheme::Gold}},
        {"Condemn",         {"Condemn", "Apply Bleed and Defense Down.\nIf transformed: give Judged + Bleed self.", 4, 4, 1, CardType::Condemn, CardTheme::Gold}},
        {"Purge",           {"Purge", "Deal damage for every unique\nstatus effect on the target.", 3, 8, 2, CardType::Purge, CardTheme::Gold}},
        {"Ritual",          {"Ritual", "Transform (Gain blessed)", 2, 4, 2, CardType::Ritual, CardTheme::Gold}},

        // FORGE FUSIONS - combine two cards!
        {"Divine Arrow",    {"Divine Arrow", "FUSION: Holy damage + Heal\nDeal damage and restore health!", 5, 5, 1, CardType::Fusion, CardTheme::Gold}},
        {"Void Storm",      {"Void Storm", "FUSION: Arcane + Void\nDeal massive damage + gain corruption!", 8, 12, 2, CardType::Fusion, CardTheme::Purple}},
        {"Beast Rampage",   {"Beast Rampage", "FUSION: Beast + Strike\nDeal big damage + gain Defence Up!", 10, 8, 2, CardType::Fusion, CardTheme::Red}},
        {"Cosmic Shield",   {"Cosmic Shield", "FUSION: Arcane + Iron\nGain massive shield + True Void!", 8, 8, 1, CardType::Fusion, CardTheme::Blue}},
        {"Blood Frenzy",    {"Blood Frenzy", "FUSION: Bleed + Attack\nDeal damage + apply bleed + heal!", 6, 10, 1, CardType::Fusion, CardTheme::Red}},
        
        // DOUBLE FUSIONS - fuse 2 fusions for WACKY results!
        {"Omega Annihilation", {"Omega Annihilation", "DOUBLE FUSION: THE END\nDeal damage to ALL enemies + apply ALL statuses + transform!", 20, 20, 3, CardType::DoubleFusion, CardTheme::Gold}},
        {"Universal Singularity", {"Universal Singularity", "DOUBLE FUSION: BLACK HOLE\nApply Void Mark x10 + True Void x10 + Corruption + Overload!", 15, 15, 2, CardType::DoubleFusion, CardTheme::Purple}},
        {"Primordial Chaos", {"Primordial Chaos", "DOUBLE FUSION: PURE CHAOS\n50% chance: Double damage OR Full heal OR Full shield OR Transform!", 25, 25, 2, CardType::DoubleFusion, CardTheme::Red}},
        {"Existential Crisis", {"Existential Crisis", "DOUBLE FUSION: REALITY BREAK\nIf enemy > 50% HP: 999 damage! Else: Heal to full + Max shield!", 50, 50, 4, CardType::DoubleFusion, CardTheme::Teal}}
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
            case CardType::Fusion:
                if (card.name == "Divine Arrow") card.effect = std::make_unique<DivineArrowFusion>();
                else if (card.name == "Void Storm") card.effect = std::make_unique<VoidStormFusion>();
                else if (card.name == "Beast Rampage") card.effect = std::make_unique<BeastRampageFusion>();
                else if (card.name == "Cosmic Shield") card.effect = std::make_unique<CosmicShieldFusion>();
                else if (card.name == "Blood Frenzy") card.effect = std::make_unique<BloodFrenzyFusion>();
                else card.effect = std::make_unique<FusionEffect>();
                break;
            case CardType::DoubleFusion:
                if (card.name == "Omega Annihilation") card.effect = std::make_unique<OmegaAnnihilation>();
                else if (card.name == "Universal Singularity") card.effect = std::make_unique<UniversalSingularity>();
                else if (card.name == "Primordial Chaos") card.effect = std::make_unique<PrimordialChaos>();
                else if (card.name == "Existential Crisis") card.effect = std::make_unique<ExistentialCrisis>();
                else card.effect = std::make_unique<FusionEffect>();
                break;

            // ranger
            case CardType::PrimalArrow:   card.effect = std::make_unique<PrimalArrowEffect>();    break;
            case CardType::Aim:           card.effect = std::make_unique<Aim>();               break;
            case CardType::ArrowVolley:    card.effect = std::make_unique<ArrowVolley>();        break;
            case CardType::Jump:           card.effect = std::make_unique<Jump>();               break;
            case CardType::Metamorphosis:  card.effect = std::make_unique<Metamorphosis>();      break;

            // mage
            case CardType::UnstableVolley: card.effect = std::make_unique<UnstableVolley>(); break;
            case CardType::EldritchBlast:  card.effect = std::make_unique<EldritchBlast>();  break;
            case CardType::VoidGrasp:      card.effect = std::make_unique<VoidGrasp>();      break;
            case CardType::AstralShift:    card.effect = std::make_unique<AstralShift>();    break;

            // warrior
            case CardType::AegisStrike:    card.effect = std::make_unique<AegisStrike>();      break;
            case CardType::IronWill:       card.effect = std::make_unique<IronWill>();         break;
            case CardType::ShatterSpleen:  card.effect = std::make_unique<ShatterSpleen>();    break;
            case CardType::ShieldBash:     card.effect = std::make_unique<ShieldBash>();       break;
            case CardType::WarriorPact:    card.effect = std::make_unique<WarriorPact>();      break;

            // cleric
            case CardType::BlindingLight: card.effect = std::make_unique<BlindingLight>();  break;
            case CardType::Condemn:       card.effect = std::make_unique<Condemn>();        break;
            case CardType::Purge:         card.effect = std::make_unique<Purge>();          break;
            case CardType::Ritual:        card.effect = std::make_unique<Ritual>();         break;
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
            case CardTheme::Gold:   return sf::Color(255, 215, 0);
            default: return sf::Color::White;
        }
    }
};
