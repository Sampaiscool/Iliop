#include "UIRenderer.h"
#include <string>

static void drawBar(sf::RenderWindow& window,
    int x,
    int y,
    int w,
    int h,
    int current,
    int max,
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

static void drawText(sf::RenderWindow& window,
    const sf::Font& font,
    const std::string& str,
    int x,
    int y,
    int charSize,
    sf::Color color)
{
    sf::Text text(font, sf::String(str), static_cast<unsigned int>(charSize));
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    window.draw(text);
}

void UIRenderer::render(sf::RenderWindow& window,
    const CombatState& playerState,
    const CombatState& enemyState,
    int winW,
    int winH,
    const sf::Font& font)
{
    int barWidth  = winW / 4;
    int barHeight = winH / 25;
    int margin    = winW / 40;

    // player
    int x = winW - barWidth - margin;
    int y = winH - (barHeight * 2) - margin * 2;

    // player corruption
    int corruptionY = y - (barHeight + 5);
    drawBar(window, x, corruptionY, barWidth, barHeight, playerState.corruption.current, playerState.corruption.max, sf::Color(118, 50, 121), sf::Color(68, 50, 66));
    drawText(window, font, std::to_string(playerState.corruption.current) + "/" + std::to_string(playerState.corruption.max), x + 5, corruptionY + 2, barHeight * 0.6, sf::Color::White);

    // player hp
    drawBar(window, x, y, barWidth, barHeight, playerState.hp.current, playerState.hp.max, sf::Color(200,50,50), sf::Color(60,20,20));
    drawText(window, font, std::to_string(playerState.hp.current) + "/" + std::to_string(playerState.hp.max), x + 5, y + 2, barHeight * 0.6, sf::Color::White);
    // player shield
    drawText(window, font, std::to_string(playerState.shield.current), x + 90, y + 2, barHeight * 0.6, sf::Color::White);

    // player mana
    drawBar(window, x, y + barHeight + margin / 2, barWidth, barHeight, playerState.mana.current, playerState.mana.max, sf::Color(50,50,200), sf::Color(20,20,60));
    drawText(window, font, std::to_string(playerState.mana.current) + "/" + std::to_string(playerState.mana.max), x + 5, y + barHeight + margin / 2 + 2, barHeight * 0.6, sf::Color::White);

    // enemy
    int enemyBarWidth  = winW / 4;
    int enemyBarHeight = barHeight;
    int enemyX = winW / 2 - enemyBarWidth / 2;
    int enemyY = winH / 4 - enemyBarHeight - margin;

    // enemy hp
    drawBar(window, enemyX, enemyY, enemyBarWidth, enemyBarHeight, enemyState.hp.current, enemyState.hp.max, sf::Color(255,0,0), sf::Color(60,20,20));
    drawText(window, font, std::to_string(enemyState.hp.current) + "/" + std::to_string(enemyState.hp.max), enemyX + 5, enemyY + 2, enemyBarHeight * 0.6, sf::Color::White);
    // enemy shield
    drawText(window, font, std::to_string(enemyState.shield.current), enemyX + 90, enemyY + 2, enemyBarHeight * 0.6, sf::Color::White);

    // end turn button
    int endButtonW = winW / 7;
    int endButtonH = endButtonW;
    int endButtonX = x + endButtonW / 3;
    int endButtonY = corruptionY - endButtonH - (winH / 40);

    endTurnBounds = sf::FloatRect(
        sf::Vector2f(
            static_cast<float>(endButtonX),
            static_cast<float>(endButtonY)),
        sf::Vector2f(
            static_cast<float>(endButtonW),
            static_cast<float>(endButtonH))
    );


    sf::RectangleShape rect(sf::Vector2f(
        static_cast<float>(endButtonW),
        static_cast<float>(endButtonH)));

    rect.setPosition(sf::Vector2f(
        static_cast<float>(endButtonX),
        static_cast<float>(endButtonY)));


    rect.setFillColor(sf::Color(50, 50, 50));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2.f);
    window.draw(rect);

    drawText(window, font, "End", endButtonX + 20, endButtonY + endButtonH / 3, barHeight * 0.8, sf::Color::White);
}

sf::FloatRect UIRenderer::getEndTurnBounds() const {
    return endTurnBounds;
}
