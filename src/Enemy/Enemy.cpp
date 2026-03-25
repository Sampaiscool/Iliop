#include "Enemy.h"
#include "../Other/AllStatuses.h"
#include "../Effects/Effect.h"
#include "../Effects/EffectTypes/Enemy/Difficulty1Skills.h"
#include "../Effects/EffectTypes/Enemy/Difficulty2Skills.h"
#include "../Effects/EffectTypes/Enemy/Difficulty3Skills.h"
#include "../Effects/EffectTypes/Enemy/Difficulty4Skills.h"
#include <map>
#include <string>

/// @brief constructs an enemy with the specified stats and type
/// @param hp the enemies health
/// @param sh the enemies shield
/// @param mana the enemies mana
/// @param cor the enemies corruption
/// @param chosenType the enemies type
Enemy::Enemy(int hp, int sh, int mana, int cor, EnemyType chosenType) {
    state.hp = {hp, hp};
    state.shield = {0, sh};
    state.mana = {mana, mana};
    state.corruption = {0, cor};
    type = chosenType;
}

/// @brief renders the enemy
/// @param window the window to draw on
/// @param winW the width of the window
/// @param winH the heigh of the window
void Enemy::render(sf::RenderWindow& window, int winW, int winH) {
    int w = winW / 4;
    int h = winW / 4;
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
        loadTexture(EnemyType::MisterEraser, "../assets/enemies/Mister_Eraser.png");
        loadTexture(EnemyType::LittleGremlin, "../assets/enemies/LittleGremlin.png");
        loadTexture(EnemyType::AngerBot, "../assets/enemies/AngerBot.png");
        loadTexture(EnemyType::Puth, "../assets/enemies/Puth.png");
        loadTexture(EnemyType::WarChief, "../assets/enemies/GoblinWarChief.png");
        loadTexture(EnemyType::FatGremlin, "../assets/enemies/FatGremlin.png");
        loadTexture(EnemyType::Bungus, "../assets/enemies/Bungus.png");
        loadTexture(EnemyType::CultistMember, "../assets/enemies/CultistMember.png");
        loadTexture(EnemyType::GiggyGrass, "../assets/enemies/GiggyGrass.png");
        loadTexture(EnemyType::BulletJunior, "../assets/enemies/BulletJunior.png");
        loadTexture(EnemyType::MagicSpright, "../assets/enemies/MagicSpright.png");
        loadTexture(EnemyType::Lihm, "../assets/enemies/Lihm.png");
        loadTexture(EnemyType::Nutou, "../assets/enemies/Nutou.png");
        loadTexture(EnemyType::MushroomMan, "../assets/enemies/MushroomMan.png");
        loadTexture(EnemyType::Biyumi, "../assets/enemies/Biyumi.png");
        loadTexture(EnemyType::FatedTraveler, "../assets/enemies/FatedTraveler.png");
        loadTexture(EnemyType::Kazan, "../assets/enemies/Kazan.png");
        loadTexture(EnemyType::DeathMarcher, "../assets/enemies/DeathMarcher2.png");
        loadTexture(EnemyType::Menta, "../assets/enemies/Menta.png");
        loadTexture(EnemyType::CursedKing, "../assets/enemies/CursedKing.png");
        loaded = true;
    }

    auto it = textures.find(type);
    if (it != textures.end()) {
        sf::Sprite spr(it->second);
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

/// @brief plays the enemies turn
/// @param playerState the player's combat state
void Enemy::playTurn(CombatState& playerState)
{
    if (intent)
        intent->apply(state, playerState, false);

    rollIntent();
}

/// @brief roll intent based on the enemies type
void Enemy::rollIntent()
{
    switch (type)
    {
        // difficulty 1
        case EnemyType::MisterEraser: {
            intent = std::make_unique<DamageEffect>(5);
            currentIntent = EnemyIntent::Attack;
            intentValue = 5;
            break;
        }
        case EnemyType::LittleGremlin: {
            intent = std::make_unique<LittleGremlinSkill>(4);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 4;
            break;
        }
        case EnemyType::AngerBot: {
            intent = std::make_unique<AngerBotSkill>(5);
            currentIntent = EnemyIntent::Attack;
            intentValue = 5;
            break;
        }
        case EnemyType::Puth: {
            intent = std::make_unique<PuthSkill>(6);
            currentIntent = EnemyIntent::Lifesteal;
            intentValue = 6;
            break;
        }
        case EnemyType::WarChief: {
            intent = std::make_unique<WarChiefSkill>(1);
            currentIntent = EnemyIntent::Buff;
            intentValue = 1;
            break;
        }

        // difficulty 2
        case EnemyType::FatGremlin: {
            intent = std::make_unique<FatGremlinSkill>(8);
            currentIntent = EnemyIntent::Heal;
            intentValue = 8;
            break;
        }
        case EnemyType::Bungus: {
            intent = std::make_unique<BungusSkill>(7);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 7;
            break;
        }
        case EnemyType::CultistMember: {
            intent = std::make_unique<CultistSkill>(8);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 8;
            break;
        }
        case EnemyType::GiggyGrass: {
            intent = std::make_unique<GiggyGrassSkill>(5);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 5;
            break;
        }
        case EnemyType::BulletJunior: {
            intent = std::make_unique<BulletJuniorSkill>(8);
            currentIntent = EnemyIntent::Block;
            intentValue = 8;
            break;
        }

        // difficulty 3
        case EnemyType::MagicSpright: {
            intent = std::make_unique<MagicSprightSkill>(10);
            currentIntent = EnemyIntent::Attack;
            intentValue = 10;
            break;
        }
        case EnemyType::Lihm: {
            intent = std::make_unique<LihmSkill>(10);
            currentIntent = EnemyIntent::Block;
            intentValue = 10;
            break;
        }
        case EnemyType::Nutou: {
            intent = std::make_unique<NutouSkill>(12);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 12;
            break;
        }
        case EnemyType::MushroomMan: {
            intent = std::make_unique<MushroomManSkill>(10);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 10;
            break;
        }
        case EnemyType::Biyumi: {
            intent = std::make_unique<BiyumiSkill>(15);
            currentIntent = EnemyIntent::Block;
            intentValue = 15;
            break;
        }

        // difficulty 4
        case EnemyType::FatedTraveler: {
            intent = std::make_unique<FatedTravelerSkill>(15);
            currentIntent = EnemyIntent::Multi;
            intentValue = 15;
            break;
        }
        case EnemyType::Kazan: {
            intent = std::make_unique<KazanSkill>(15);
            currentIntent = EnemyIntent::Attack;
            intentValue = 15;
            break;
        }
        case EnemyType::DeathMarcher: {
            intent = std::make_unique<DeathMarcherSkill>(20);
            currentIntent = EnemyIntent::Multi;
            intentValue = 20;
            break;
        }
        case EnemyType::Menta: {
            intent = std::make_unique<MentaSkill>(15);
            currentIntent = EnemyIntent::Debuff;
            intentValue = 15;
            break;
        }
        case EnemyType::CursedKing: {
            intent = std::make_unique<CursedKingSkill>(20);
            currentIntent = EnemyIntent::Multi;
            intentValue = 20;
            break;
        }
    }
}

/// @brief gets the description based of the enemies intent
/// @return the description of the intent
std::string Enemy::getIntentDescription() const {
    switch (currentIntent) {
        case EnemyIntent::Attack: return "Attack " + std::to_string(intentValue);
        case EnemyIntent::Block: return "Block " + std::to_string(intentValue);
        case EnemyIntent::Heal: return "Heal " + std::to_string(intentValue);
        case EnemyIntent::Buff: return "Buff";
        case EnemyIntent::Debuff: return "Debuff " + std::to_string(intentValue);
        case EnemyIntent::Multi: return "Special " + std::to_string(intentValue);
        case EnemyIntent::Lifesteal: return "Lifesteal " + std::to_string(intentValue);
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
