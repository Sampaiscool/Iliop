#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../Other/CombatState.h"
#include "../Effects/Effect.h"
#include "../Effects/EffectTypes/DamageEffect.h"
#include "../Effects/EffectTypes/HealEffect.h"
#include "../Effects/EffectTypes/ShieldEffect.h"
#include "../Effects/MultiEffect.h"

enum class EnemyType {
    MisterEraser,
    Menta,
    CursedKing
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

private:
    CombatState state;
    EnemyType type;
    std::unique_ptr<Effect> intent;
};
