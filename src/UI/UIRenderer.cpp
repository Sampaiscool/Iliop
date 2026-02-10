#include "UIRenderer.h"
#include <string>

static void drawBar(sf::RenderWindow& window, int x, int y, int w, int h, int current, int max,
                    sf::Color fill, sf::Color back)
{
    sf::RectangleShape bg(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
    bg.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    bg.setFillColor(back);
    bg.setOutlineColor(sf::Color::Black);
    bg.setOutlineThickness(2.f);
    window.draw(bg);

    float pct = static_cast<float>(current) / static_cast<float>(max);
    sf::RectangleShape fg(sf::Vector2f(w * pct, static_cast<float>(h)));
    fg.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    fg.setFillColor(fill);
    window.draw(fg);
}

static void drawText(sf::RenderWindow& window, const sf::Font& font, const std::string& str,
                     int x, int y, int charSize, sf::Color color)
{
    sf::Text text(font, sf::String(str), static_cast<unsigned int>(charSize));
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    window.draw(text);
}

void UIRenderer::render(sf::RenderWindow& window, const CombatState& state,
                        int winW, int winH, const sf::Font& font)
{
    int barWidth  = winW / 4;
    int barHeight = winH / 25;
    int margin    = winW / 40;

    // player
    int x = winW - barWidth - margin;
    int y = winH - (barHeight * 2) - margin * 2;

    drawBar(window, x, y, barWidth, barHeight, state.hp.current, state.hp.max, sf::Color(200,50,50), sf::Color(60,20,20));
    drawText(window, font, std::to_string(state.hp.current) + "/" + std::to_string(state.hp.max), x + 5, y + 2, barHeight * 0.6, sf::Color::White);

    drawBar(window, x, y + barHeight + margin / 2, barWidth, barHeight, state.mana.current, state.mana.max, sf::Color(50,50,200), sf::Color(20,20,60));
    drawText(window, font, std::to_string(state.mana.current) + "/" + std::to_string(state.mana.max), x + 5, y + barHeight + margin / 2 + 2, barHeight * 0.6, sf::Color::White);

    // enemy
    int enemyBarWidth  = winW / 4;
    int enemyBarHeight = barHeight;
    int enemyX = winW / 2 - enemyBarWidth / 2;
    int enemyY = winH / 4 - enemyBarHeight - margin;

    drawBar(window, enemyX, enemyY, enemyBarWidth, enemyBarHeight, state.hp.current, state.hp.max, sf::Color(255,0,0), sf::Color(60,20,20));
    drawText(window, font, std::to_string(state.hp.current) + "/" + std::to_string(state.hp.max), enemyX + 5, enemyY + 2, enemyBarHeight * 0.6, sf::Color::White);
}

