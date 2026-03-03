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

// enemy types are driven by the Doobies Gist, plus additional custom creations
// when expanding this list, be sure to update Enemy::rollIntent and EnemyFactory
enum class EnemyType {
    // early floors
    MisterEraser,
    LittleGremlin,
    AngerBot,
    Puth,

    // mid floors
    FatGremlin,
    Bungus,
    CultistMember,
    MagicSpright,
    Lihm,
    Nutou,
    MushroomMan,

    // endgame / bosses
    FatedTraveler,
    Kazan,
    DeathMarcher,
    Biyumi,

    // original custom enemies
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
