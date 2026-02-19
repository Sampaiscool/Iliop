#include "UIRenderer.h"
#include <string>
#include <sstream>

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
    drawText(window, font, std::to_string(playerState.shield.current) + "/" + std::to_string(playerState.shield.max), x + (winW / 7), y + 2, barHeight * 0.6, sf::Color::White);

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
    drawText(window, font, std::to_string(enemyState.shield.current), enemyX + (winW / 7), enemyY + 2, enemyBarHeight * 0.6, sf::Color::White);

    int btnW = winW / 10;
    int btnH = barHeight * 2;
    int buttonsY = corruptionY - btnH - (winH / 60);
    int btnSpacing = 10;

    // end turn button
    int endX = x + barWidth - btnW;

    endTurnBounds = sf::FloatRect(
        sf::Vector2f(static_cast<float>(endX), static_cast<float>(buttonsY)),
        sf::Vector2f(static_cast<float>(btnW), static_cast<float>(btnH))
    );

    sf::RectangleShape rectEnd(sf::Vector2f(btnW, btnH));
    rectEnd.setPosition(sf::Vector2f(static_cast<float>(endX), static_cast<float>(buttonsY)));
    rectEnd.setFillColor(sf::Color(100, 100, 100)); 
    rectEnd.setOutlineColor(sf::Color::Black);
    rectEnd.setOutlineThickness(2.f);
    window.draw(rectEnd);

    drawText(window, font, "End", endX + (btnW / 4), buttonsY + (btnH / 4), btnH * 0.5, sf::Color::White);

    // transform button
    int transX = x;

    transformBounds = sf::FloatRect(
        sf::Vector2f(static_cast<float>(transX), static_cast<float>(buttonsY)),
        sf::Vector2f(static_cast<float>(btnW), static_cast<float>(btnH))
    );

    sf::RectangleShape rectTransform(sf::Vector2f(static_cast<float>(btnW), static_cast<float>(btnH)));
    rectTransform.setPosition(sf::Vector2f(static_cast<float>(transX), static_cast<float>(buttonsY)));

    if (playerState.mana.current == playerState.mana.max) {
        rectTransform.setFillColor(sf::Color(244, 198, 68));
    } else {
        rectTransform.setFillColor(sf::Color(100, 80, 20));
    }

    rectTransform.setOutlineColor(sf::Color::Black);
    rectTransform.setOutlineThickness(2.f);
    window.draw(rectTransform);
    drawText(window, font, "Form", transX + (btnW / 6), buttonsY + (btnH / 5), btnH * 0.6, sf::Color::Black);
}

void UIRenderer::drawTooltip(sf::RenderWindow& window, const sf::Font& font, const Card& card, float mouseX, float mouseY) {
    std::ostringstream oss;
    oss << card.name << "\n";
    oss << "Cost: " << card.cost << " | Value: " << card.value;
    // if you are corrupted show the corrupted value
    if (card.corruptedValue > 0) {
        oss << " (+" << card.corruptedValue << " Corrupted)";
    }
    
    oss << "\n------------------\n" 
        << card.description;

    sf::Text descText(font, sf::String(oss.str()), 16);
    descText.setFillColor(sf::Color::White);

    // get the background and shape
    sf::FloatRect bounds = descText.getLocalBounds();
    sf::Vector2f boxSize(bounds.size.x + 20.f, bounds.size.y + 25.f);
    sf::RectangleShape bg(boxSize);

    sf::Vector2f position(mouseX + 15.f, mouseY + 15.f);
    
    // this way the toolbox cant get out of bounds
    if (position.x + boxSize.x > window.getSize().x) {
        position.x = mouseX - boxSize.x - 15.f;
    }
    if (position.y + boxSize.y > window.getSize().y) {
        position.y = mouseY - boxSize.y - 15.f;
    }

    bg.setPosition(position);
    bg.setFillColor(sf::Color(15, 15, 20, 240)); // blue-tinted dark grey
    bg.setOutlineColor(sf::Color(118, 50, 121)); // purple corruption color
    bg.setOutlineThickness(2.f);

    descText.setPosition({position.x + 10.f, position.y + 5.f});

    window.draw(bg);
    window.draw(descText);
}

sf::FloatRect UIRenderer::getEndTurnBounds() const {
    return endTurnBounds;
}

sf::FloatRect UIRenderer::getTransformBounds() const {
    return transformBounds;
}
