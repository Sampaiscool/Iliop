#include "Enemy.h"

Enemy::Enemy(int hp, int sh, int mana, int cor) {
    state = {
        {hp, hp},     // hp
        {0, sh},      // shield
        {mana, mana}, // mana
        {0, cor}      // corruption
    };
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
