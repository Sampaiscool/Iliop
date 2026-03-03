#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "../Other/CombatState.h"
#include "../Effects/Effect.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include "../Effects/MultiEffect.h"

enum class EnemyType {
    MisterEraser,
    LittleGremlin,
    AngerBot,
    Puth,

    FatGremlin,
    Bungus,
    CultistMember,
    MagicSpright,
    Lihm,
    Nutou,
    MushroomMan,

    FatedTraveler,
    Kazan,
    DeathMarcher,
    Biyumi,

    Menta,
    CursedKing,
    VoidSpider,
    FlameImp,
    FrostWraith,
    ShadowAssassin,
    BloodGolem,
    ArcaneGolem,
    SoulReaper
};

enum class EnemyIntent {
    Attack,
    Block,
    Buff,
    Debuff,
    Heal,
    Multi
};

class Enemy {
public:
    Enemy(int hp, int sh, int mana, int cor, EnemyType chosenType);
    void render(sf::RenderWindow& window, int winW, int winH);
    CombatState& getState();
    void playTurn(CombatState& playerState);
    void rollIntent();
    int getIntentDamage() const;
    int getIntentBlock() const;
    EnemyIntent getIntentType() const { return currentIntent; }
    std::string getIntentDescription() const;

private:
    CombatState state;
    EnemyType type;
    EnemyIntent currentIntent = EnemyIntent::Attack;
    std::unique_ptr<Effect> intent;
    int intentValue = 0;
};
