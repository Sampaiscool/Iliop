#include "Enemy.h"
#include "../Other/AllStatuses.h"
#include "../Effects/Effect.h"

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

    sf::RectangleShape rect(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
    rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    rect.setFillColor(sf::Color(255, 0, 255));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2.f);
    window.draw(rect);
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
            break;
        }

        case EnemyType::Menta:
        {
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(5));
            multi->add(std::make_unique<ShieldEffect>(2));
            intent = std::move(multi);
            break;
        }

        case EnemyType::CursedKing:
        {
            auto multi = std::make_unique<MultiEffect>();
            multi->add(std::make_unique<DamageEffect>(10));
            multi->add(std::make_unique<HealEffect>(5));
            intent = std::move(multi);
            break;
        }
    }
}

