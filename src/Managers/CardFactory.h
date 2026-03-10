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

// necromancer:
#include "../Effects/EffectTypes/Necromancer/GraveCall.h"
#include "../Effects/EffectTypes/Necromancer/BoneSplinter.h"
#include "../Effects/EffectTypes/Necromancer/RottingBite.h"
#include "../Effects/EffectTypes/Necromancer/SoulHarvest.h"
#include "../Effects/EffectTypes/Necromancer/DeathMark.h"
#include "../Effects/EffectTypes/Necromancer/ArmySurge.h"
#include "../Effects/EffectTypes/Necromancer/CryptShield.h"
#include "../Effects/EffectTypes/Necromancer/RaiseDead.h"
#include "../Effects/EffectTypes/Necromancer/BoneLegion.h"

// alchemist:
#include "../Effects/EffectTypes/Alchemist/PotionBrew.h"
#include "../Effects/EffectTypes/Alchemist/Toss.h"
#include "../Effects/EffectTypes/Alchemist/Reaction.h"
#include "../Effects/EffectTypes/Alchemist/Copper.h"
#include "../Effects/EffectTypes/Alchemist/Lead.h"
#include "../Effects/EffectTypes/Alchemist/Gold.h"
#include "../Effects/EffectTypes/Alchemist/Iron.h"
#include "../Effects/EffectTypes/Alchemist/Mercury.h"
#include "../Effects/EffectTypes/Alchemist/Silver.h"
#include "../Effects/EffectTypes/Alchemist/Tin.h"

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
        {"Arrow Volley",    {"Arrow Volley", "Fire a volley of arrows.\n(More hits + chance to bleed)", 3, 3, 1, CardType::ArrowVolley, CardTheme::Red}},
        {"Jump",            {"Jump", "Shield and Defence Up.\n(Deal damage + Defence Up)", 2, 2, 1, CardType::Jump, CardTheme::Teal}},
        {"Metamorphosis",   {"Metamorphosis", "Transform.\n(Extend transform + heal)", 1, 2, 1, CardType::Metamorphosis, CardTheme::Green}},

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
        {"Purge",           {"Purge", "Deal damage for every unique\nstatus effect on the target.", 3, 8, 2, CardType::Purge, CardTheme::Red}},
        {"Ritual",          {"Ritual", "Transform (Gain blessed)", 2, 4, 2, CardType::Ritual, CardTheme::Green}},

        // necromancer
        {"Raise Dead",      {"Raise Dead", "Summon Zombie Army.\nTransform: Double zombies + heal", 1, 1, 1, CardType::RaiseDead, CardTheme::Gold}},
        {"Bone Legion",     {"Bone Legion", "Summon Skeleton Army.\nTransform: Double skeletons + shield", 1, 1, 1, CardType::BoneLegion, CardTheme::Gold}},
        {"Bone Splinter",   {"Bone Splinter", "Deal damage + skeleton bonus.\n(+ Apply Defence Down + Soul)", 1, 2, 1, CardType::BoneSplinter, CardTheme::Red}},
        {"Rotting Bite",    {"Rotting Bite", "Deal damage + zombie bonus + Bleed.\nHeal from zombies. (+ Death Mark)", 1, 2, 1, CardType::RottingBite, CardTheme::Red}},
        {"Soul Harvest",    {"Soul Harvest", "Gain Soul Fragments.\n10 Fragments: Build 1 Zombie + 1 Skeleton", 2, 3, 1, CardType::SoulHarvest, CardTheme::Purple}},
        {"Death Mark",      {"Death Mark", "Mark enemy: Extra damage taken.\n(Double marks + Bleed)", 1, 1, 1, CardType::DeathMark, CardTheme::Red}},
        {"Army Surge",      {"Army Surge", "Transform + Army + Soul\n (Increased army boost)", 2, 4, 3, CardType::ArmySurge, CardTheme::Green}},
        {"Crypt Shield",    {"Crypt Shield", "Shield + skeleton bonus.\n(+ Extra shield + Defence Up)", 0, 2, 1, CardType::CryptShield, CardTheme::Gray}},

        // alchemist
        {"Potion Brew",     {"Potion Brew", "Create 2 random metals.", 0, 0, 1, CardType::PotionBrew, CardTheme::Blue}},
        {"Toss",            {"Toss", "Remove 1 metal and deal increased damage\nbased on its value", 3, 7, 1, CardType::Toss, CardTheme::Red}},
        {"Reaction",        {"Reaction", "Mix 2 metals for a powerful effect", 8, 3, 0, CardType::Reaction, CardTheme::Purple}},
        {"Lead",            {"Lead", "Apply Lead status", 3, 0, 1, CardType::LeadCard, CardTheme::Gray}},
        {"Gold",            {"Gold", "Apply Gold status", 4, 0, 1, CardType::GoldCard, CardTheme::Gray}},
        {"Copper",          {"Copper", "Apply Copper status", 2, 0, 1, CardType::CopperCard, CardTheme::Gray}},
        {"Iron",            {"Iron", "Apply Iron status", 3, 0, 1, CardType::IronCard, CardTheme::Gray}},
        {"Mercury",         {"Mercury", "Apply Mercury status", 2, 0, 1, CardType::MercuryCard, CardTheme::Gray}},
        {"Silver",          {"Silver", "Apply Silver status", 3, 0, 1, CardType::SilverCard, CardTheme::Gray}},
        {"Tin",             {"Tin", "Apply Tin status", 1, 0, 1, CardType::TinCard, CardTheme::Gray}},

        // forge fusion
        {"Divine Arrow",    {"Divine Arrow", "FUSION: Holy damage + Heal\nDeal damage and restore health!", 5, 5, 1, CardType::Fusion, CardTheme::Gold}},
        {"Void Storm",      {"Void Storm", "FUSION: Arcane + Void\nDeal massive damage + gain corruption!", 8, 12, 2, CardType::Fusion, CardTheme::Purple}},
        {"Beast Rampage",   {"Beast Rampage", "FUSION: Beast + Strike\nDeal big damage + gain Defence Up!", 10, 8, 2, CardType::Fusion, CardTheme::Red}},
        {"Cosmic Shield",   {"Cosmic Shield", "FUSION: Arcane + Iron\nGain massive shield + True Void!", 8, 8, 1, CardType::Fusion, CardTheme::Blue}},
        {"Blood Frenzy",    {"Blood Frenzy", "FUSION: Bleed + Attack\nDeal damage + apply bleed + heal!", 6, 10, 1, CardType::Fusion, CardTheme::Red}},

        // double fusions
        {"Omega Annihilation", {"Omega Annihilation", "DOUBLE FUSION: THE END\nDeal insane damage \napply ALL statuses + transform!", 20, 20, 4, CardType::DoubleFusion, CardTheme::Gold}},
        {"Universal Singularity", {"Universal Singularity", "DOUBLE FUSION: BLACK HOLE\nApply Void Mark x10 + True Void x10\n And corruption + Overload!", 15, 15, 2, CardType::DoubleFusion, CardTheme::Purple}},
        {"Primordial Chaos", {"Primordial Chaos", "DOUBLE FUSION: PURE CHAOS\n50% chance: Double damage OR Full heal \nOR Full shield OR Transform!", 25, 25, 2, CardType::DoubleFusion, CardTheme::Red}},
        {"Existential Crisis", {"Existential Crisis", "DOUBLE FUSION: REALITY BREAK\nIf enemy > 50% HP: 999 damage!\nElse: Heal to full + Max shield!", 50, 50, 4, CardType::DoubleFusion, CardTheme::Teal}}
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

            // necromancer
            case CardType::RaiseDead:     card.effect = std::make_unique<RaiseDead>();      break;
            case CardType::BoneLegion:    card.effect = std::make_unique<BoneLegion>();     break;
            case CardType::BoneSplinter:  card.effect = std::make_unique<BoneSplinter>();   break;
            case CardType::RottingBite:   card.effect = std::make_unique<RottingBite>();    break;
            case CardType::SoulHarvest:    card.effect = std::make_unique<SoulHarvest>();    break;
            case CardType::DeathMark:     card.effect = std::make_unique<DeathMark>();      break;
            case CardType::ArmySurge:     card.effect = std::make_unique<ArmySurge>();      break;
            case CardType::CryptShield:   card.effect = std::make_unique<CryptShield>();    break;

            // alchemist
            case CardType::PotionBrew:   card.effect = std::make_unique<PotionBrew>();     break;
            case CardType::Toss:         card.effect = std::make_unique<Toss>();           break;
            case CardType::Reaction:      card.effect = std::make_unique<Reaction>();       break;
            case CardType::LeadCard:      card.effect = std::make_unique<LeadEffect>();      break;
            case CardType::GoldCard:      card.effect = std::make_unique<GoldEffect>();      break;
            case CardType::CopperCard:    card.effect = std::make_unique<Copper>();          break;
            case CardType::IronCard:      card.effect = std::make_unique<IronEffect>();      break;
            case CardType::MercuryCard:   card.effect = std::make_unique<MercuryEffect>();   break;
            case CardType::SilverCard:    card.effect = std::make_unique<SilverEffect>();    break;
            case CardType::TinCard:       card.effect = std::make_unique<TinEffect>();      break;
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
