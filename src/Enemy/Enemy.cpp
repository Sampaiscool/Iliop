#include "Enemy.h"
#include "../Other/AllStatuses.h"
#include "../Effects/Effect.h"
#include "../Effects/EffectTypes/Enemy/EnemyPoisonStrike.h"
#include "../Effects/EffectTypes/Enemy/EnemySelfBuff.h"
#include "../Effects/EffectTypes/Enemy/EnemyWeakening.h"
#include "../Effects/EffectTypes/Enemy/EnemyDefensiveStrike.h"
#include <map>
#include <string>

Enemy::Enemy(int hp, int sh, int mana, int cor, EnemyType chosenType) {
    state.hp = {hp, hp};
    state.shield = {0, sh};
    state.mana = {mana, mana};
    state.corruption = {0, cor};
    type = chosenType;
}

void Enemy::render(sf::RenderWindow& window, int winW, int winH) {
    int w = winW / 4;
    int h = winH / 4;
    int x = winW / 2 - w / 2;
    int y = winH / 4;

    // lazy load the enemies
    static std::map<EnemyType, sf::Texture> textures;
    static bool loaded = false;
    auto loadTexture = [&](EnemyType et, const std::string& filename) {
        if (textures.find(et) != textures.end()) return;
        sf::Texture tex;
        if (tex.loadFromFile(filename)) {
            textures.emplace(et, std::move(tex));
        }
    };

    if (!loaded) {
        // this is a list, yup 100% a list that existst
        loadTexture(EnemyType::MisterEraser, "../assets/enemies/Mister_Eraser.png");
        loadTexture(EnemyType::LittleGremlin, "../assets/enemies/LittleGremlin.png");
        loadTexture(EnemyType::AngerBot, "../assets/enemies/AngerBot.png");
        loadTexture(EnemyType::Puth, "../assets/enemies/Puth.png");
        loadTexture(EnemyType::FatGremlin, "../assets/enemies/FatGremlin.png");
        loadTexture(EnemyType::Bungus, "../assets/enemies/Bungus.png");
        loadTexture(EnemyType::CultistMember, "../assets/enemies/CultistMember.png");
        loadTexture(EnemyType::MagicSpright, "../assets/enemies/MagicSpright.png");
        loadTexture(EnemyType::Lihm, "../assets/enemies/Lihm.png");
        loadTexture(EnemyType::Nutou, "../assets/enemies/Nutou.png");
        loadTexture(EnemyType::MushroomMan, "../assets/enemies/MushroomMan.png");
        loadTexture(EnemyType::FatedTraveler, "../assets/enemies/FatedTraveler.png");
        loadTexture(EnemyType::Kazan, "../assets/enemies/Kazan.png");
        loadTexture(EnemyType::DeathMarcher, "../assets/enemies/DeathMarcher2.png");
        loadTexture(EnemyType::Biyumi, "../assets/enemies/Biyumi.png");
        loadTexture(EnemyType::Menta, "../assets/enemies/Mantra.png");
        loadTexture(EnemyType::CursedKing, "../assets/enemies/CultistLeader.png");
        loadTexture(EnemyType::VoidSpider, "../assets/enemies/B.png");
        loadTexture(EnemyType::FlameImp, "../assets/enemies/MightyFire.png");
        loadTexture(EnemyType::FrostWraith, "../assets/enemies/Mantra-export.png");
        loadTexture(EnemyType::ShadowAssassin, "../assets/enemies/DeathMarcher2.png");
        loadTexture(EnemyType::BloodGolem, "../assets/enemies/GoblinWarChief.png");
        loadTexture(EnemyType::ArcaneGolem, "../assets/enemies/GiggyGrass.png");
        loadTexture(EnemyType::SoulReaper, "../assets/enemies/FatedTraveler.png");
        loaded = true;
    }

    auto it = textures.find(type);
    if (it != textures.end()) {
        sf::Sprite spr(it->second);
        // fit sprite into rectangle while not messing up the sprite
        float texW = static_cast<float>(it->second.getSize().x);
        float texH = static_cast<float>(it->second.getSize().y);
        float sx = static_cast<float>(w) / texW;
        float sy = static_cast<float>(h) / texH;
        float s = std::min(sx, sy);
        spr.setScale(sf::Vector2f(s, s));
        spr.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        window.draw(spr);
    } else {
        sf::RectangleShape rect(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
        rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
        rect.setFillColor(sf::Color(255, 0, 255));
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2.f);
        window.draw(rect);
    }
}

CombatState& Enemy::getState() {
    return state;
}

void Enemy::playTurn(CombatState& playerState)
{
    if (intent)
        intent->apply(state, playerState, false);

    rollIntent();
}

void Enemy::rollIntent()
{
    switch (type)
    {
        case EnemyType::MisterEraser:
        {
            intent = std::make_unique<DamageEffect>(5);
            currentIntent = EnemyIntent::Attack;
            intentValue = 5;
            break;
        }

        case EnemyType::LittleGremlin:
        {
            intent = std::make_unique<DamageEffect>(3);
            currentIntent = EnemyIntent::Attack;
            intentValue = 3;
            break;
        }

        case EnemyType::AngerBot:
        {
            intent = std::make_unique<DamageEffect>(7);
            currentIntent = EnemyIntent::Attack;
            intentValue = 7;
            break;
        }

        case EnemyType::Puth:
        {
            // debuffs player – represented here as a weak attack TODO: you know it
            intent = std::make_unique<DamageEffect>(5);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 5;
            break;
        }

        case EnemyType::FatGremlin:
        {
            intent = std::make_unique<ShieldEffect>(10);
            currentIntent = EnemyIntent::Block;
            intentValue = 10;
            break;
        }

        case EnemyType::Bungus:
        {
            intent = std::make_unique<HealEffect>(10);
            currentIntent = EnemyIntent::Heal;
            intentValue = 10;
            break;
        }

        case EnemyType::CultistMember:
        {
            // cultists buff themselves while attacking
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(8));
            multi->add(std::make_unique<ShieldEffect>(4));
            intent = std::move(multi);
            currentIntent = EnemyIntent::Attack;
            intentValue = 8;
            break;
        }

        case EnemyType::MagicSpright:
        {
            // spirits alternate between attacking and buffing
            if (rand() % 2 == 0) {
                intent = std::make_unique<DamageEffect>(6);
                currentIntent = EnemyIntent::Attack;
                intentValue = 6;
            } else {
                intent = std::make_unique<ShieldEffect>(8);
                currentIntent = EnemyIntent::Block;
                intentValue = 8;
            }
            break;
        }

        case EnemyType::Lihm:
        {
            // lihm is balanced attack and defense
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(9));
            multi->add(std::make_unique<ShieldEffect>(3));
            intent = std::move(multi);
            currentIntent = EnemyIntent::Attack;
            intentValue = 9;
            break;
        }

        case EnemyType::Nutou:
        {
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(5));
            multi->add(std::make_unique<ShieldEffect>(5));
            intent = std::move(multi);
            currentIntent = EnemyIntent::Multi;
            intentValue = 5;
            break;
        }

        case EnemyType::MushroomMan:
        {
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<ShieldEffect>(5));
            multi->add(std::make_unique<HealEffect>(5));
            intent = std::move(multi);
            currentIntent = EnemyIntent::Multi;
            intentValue = 5;
            break;
        }

        case EnemyType::Menta:
        {
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(5));
            multi->add(std::make_unique<ShieldEffect>(2));
            intent = std::move(multi);
            currentIntent = EnemyIntent::Multi;
            intentValue = 5;
            break;
        }

        case EnemyType::CursedKing:
        {
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(10));
            multi->add(std::make_unique<HealEffect>(5));
            intent = std::move(multi);
            currentIntent = EnemyIntent::Multi;
            intentValue = 10;
            break;
        }
        case EnemyType::VoidSpider:
        {
            // poisonous spider strikes and applies bleed
            intent = std::make_unique<EnemyPoisonStrike>(8);
            currentIntent = EnemyIntent::Attack;
            intentValue = 8;
            break;
        }
        
        case EnemyType::FlameImp:
        {
            intent = std::make_unique<DamageEffect>(12);
            currentIntent = EnemyIntent::Attack;
            intentValue = 12;
            break;
        }
        
        case EnemyType::FrostWraith:
        {
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(7));
            multi->add(std::make_unique<ShieldEffect>(5));
            intent = std::move(multi);
            currentIntent = EnemyIntent::Block;
            intentValue = 7;
            break;
        }
        
        case EnemyType::ShadowAssassin:
        {
            // weakens the target while striking
            intent = std::make_unique<EnemyWeakening>(15);
            currentIntent = EnemyIntent::Attack;
            intentValue = 15;
            break;
        }
        
        case EnemyType::BloodGolem:
        {
            // Heals itself and gains defense buff
            intent = std::make_unique<EnemySelfBuff>(8);
            currentIntent = EnemyIntent::Heal;
            intentValue = 8;
            break;
        }
        
        case EnemyType::ArcaneGolem:
        {
            // Shields heavily and weakens player defence
            intent = std::make_unique<EnemyDefensiveStrike>(15);
            currentIntent = EnemyIntent::Block;
            intentValue = 15;
            break;
        }
        
        case EnemyType::SoulReaper:
        {
            int roll = rand() % 3;
            if (roll == 0) {
                intent = std::make_unique<DamageEffect>(20);
                currentIntent = EnemyIntent::Attack;
                intentValue = 20;
            } else if (roll == 1) {
                auto multi = std::make_unique<MultiEffect>();
                multi->add(std::make_unique<DamageEffect>(10));
                multi->add(std::make_unique<DamageEffect>(10));
                intent = std::move(multi);
                currentIntent = EnemyIntent::Attack;
                intentValue = 20;
            } else {
                auto multi = std::make_unique<MultiEffect>();
                multi->add(std::make_unique<DamageEffect>(8));
                multi->add(std::make_unique<DamageEffect>(8));
                intent = std::move(multi);
                currentIntent = EnemyIntent::Debuff;
                intentValue = 8;
            }
            break;
        }
        default:
        {
            // fallback :)
            intent = std::make_unique<DamageEffect>(1);
            currentIntent = EnemyIntent::Attack;
            intentValue = 1;
            break;
        }
    }
}

std::string Enemy::getIntentDescription() const {
    switch (currentIntent) {
        case EnemyIntent::Attack: return "Attack " + std::to_string(intentValue);
        case EnemyIntent::Block: return "Block " + std::to_string(intentValue);
        case EnemyIntent::Heal: return "Heal " + std::to_string(intentValue);
        case EnemyIntent::Buff: return "Buff";
        case EnemyIntent::Debuff: return "Debuff " + std::to_string(intentValue);
        case EnemyIntent::Multi: return "Multi " + std::to_string(intentValue);
        default: return "?";
    }
}

int Enemy::getIntentDamage() const {
    if (currentIntent == EnemyIntent::Attack || currentIntent == EnemyIntent::Multi || currentIntent == EnemyIntent::Debuff) {
        return intentValue;
    }
    return 0;
}

int Enemy::getIntentBlock() const {
    if (currentIntent == EnemyIntent::Block) {
        return intentValue;
    }
    return 0;
}
