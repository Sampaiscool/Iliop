#include "UIRenderer.h"
#include <string>
#include <sstream>
#include <iostream>

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

sf::Color getClassColor(Class type) {
    switch (type) {
        case Class::Mage:    return sf::Color(50, 150, 255); // blue
        case Class::Warrior: return sf::Color(255, 50, 50);  // red
        case Class::Cleric:  return sf::Color(255, 255, 150); // gold/yellow
        default:             return sf::Color::White;
    }
}

void drawPlayerPortrait(sf::RenderWindow& window, const sf::Font& font, const Character& player, int winW, int winH, const std::map<CharacterName, sf::Texture>& portraitTextures) {
    float boxW = winW / 8.f;
    float boxH = winW / 8.f;
    float margin = 20.f;

    float x = margin;
    float y = winH - boxH - (winH / 3.f); 

    sf::RectangleShape rect(sf::Vector2f(boxW, boxH));
    rect.setPosition(sf::Vector2f(x, y));

    // set border based on class
    rect.setOutlineColor(getClassColor(player.type));
    rect.setOutlineThickness(4.f);

    // set texture based on sprite
    auto it = portraitTextures.find(player.identity);
    if (it != portraitTextures.end()) {
        rect.setTexture(&it->second);
        rect.setFillColor(sf::Color::White);
    } else {
        rect.setFillColor(sf::Color(30, 30, 30)); // fallback
    }

    window.draw(rect);

    drawText(window, font, player.nameStr, x, y - 25, 18, sf::Color::White);
}

static const Status* drawStatusIcons(sf::RenderWindow& window, const sf::Font& font,
    const std::vector<std::unique_ptr<Status>>& statuses,
    int x, int y, sf::Vector2f mousePos, const std::map<StatusType, sf::Texture>& textures)
{
    float iconSize = 30.f;
    float spacing = 6.f;
    const Status* hoveredStatus = nullptr;

    for (size_t i = 0; i < statuses.size(); ++i) {
        sf::Vector2f pos(static_cast<float>(x) + (i * (iconSize + spacing)), static_cast<float>(y));
        sf::RectangleShape box(sf::Vector2f(iconSize, iconSize));
        box.setPosition(pos);

        // sprite handling
        auto it = textures.find(statuses[i]->getType());
        if (it != textures.end()) {
            box.setTexture(&it->second);
        } else {
            box.setFillColor(sf::Color(50, 50, 50)); // fallback
        }

        window.draw(box);

        if (box.getGlobalBounds().contains(mousePos)) {
            hoveredStatus = statuses[i].get();
        }

        sf::Text intensText(font, sf::String(std::to_string(statuses[i]->intensity)), 12);
        intensText.setFillColor(sf::Color::Yellow);
        intensText.setOutlineColor(sf::Color::Black);
        intensText.setOutlineThickness(1.f);
        intensText.setPosition(sf::Vector2f(pos.x + iconSize - 12.f, pos.y + iconSize - 14.f));
        window.draw(intensText);
    }
    return hoveredStatus;
}
void UIRenderer::render(sf::RenderWindow& window,
    const Character& player,
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
    lastPlayerBarPos = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));

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
    lastEnemyBarPos = sf::Vector2f(static_cast<float>(enemyX), static_cast<float>(enemyY));

    // enemy hp
    drawBar(window, enemyX, enemyY, enemyBarWidth, enemyBarHeight, enemyState.hp.current, enemyState.hp.max, sf::Color(255,0,0), sf::Color(60,20,20));
    drawText(window, font, std::to_string(enemyState.hp.current) + "/" + std::to_string(enemyState.hp.max), enemyX + 5, enemyY + 2, enemyBarHeight * 0.6, sf::Color::White);

    // enemy shield
    drawText(window, font, std::to_string(enemyState.shield.current), enemyX + (winW / 7), enemyY + 2, enemyBarHeight * 0.6, sf::Color::White);

    int btnW = winW / 10;
    int btnH = barHeight * 2;
    int buttonsY = corruptionY - btnH - (winH / 60);
    int btnSpacing = 10;

    drawPlayerPortrait(window, font, player, winW, winH, portraitTextures);

    // skips the first frame check
    if (lastPlayerHP == -1) {
        lastPlayerHP = playerState.hp.current;
        lastEnemyHP = enemyState.hp.current;
        return;
    }

    // detects player hp changes
    if (playerState.hp.current != lastPlayerHP) {
        int diff = playerState.hp.current - lastPlayerHP;
        sf::Color color = (diff > 0) ? sf::Color::Green : sf::Color::Red;
        std::string text = (diff > 0) ? "+" + std::to_string(diff) : std::to_string(diff);
        spawnFCT(lastPlayerBarPos, text, color, font);
        lastPlayerHP = playerState.hp.current;
    }

    // same goes for the enemy
    if (enemyState.hp.current != lastEnemyHP) {
        int diff = enemyState.hp.current - lastEnemyHP;
        sf::Color color = (diff > 0) ? sf::Color::Green : sf::Color::Red;
        std::string text = (diff > 0) ? "+" + std::to_string(diff) : std::to_string(diff);
        spawnFCT(lastEnemyBarPos, text, color, font);
        lastEnemyHP = enemyState.hp.current;
    }

    // get thah mouse position
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // player status TODO: check if it works?
    int playerStatusX = x;
    int playerStatusY = corruptionY - barHeight - (winH / 8); 
    const Status* hovered = drawStatusIcons(window, font, playerState.statuses, 
                                           playerStatusX, playerStatusY, mousePos, statusTextures);

    // enemy status
    int enemyStatusX = enemyX;
    int enemyStatusY = enemyY - (enemyY / 2 - 10);
    const Status* enemyHovered = drawStatusIcons(window, font, enemyState.statuses, 
                                                enemyStatusX, enemyStatusY, mousePos, statusTextures);

    // if we are hovering draw tooltip last
    if (hovered) {
        drawStatusTooltip(window, font, *hovered, mousePos);
    } else if (enemyHovered) {
        drawStatusTooltip(window, font, *enemyHovered, mousePos);
    }

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

void UIRenderer::spawnFCT(sf::Vector2f pos, std::string str, sf::Color color, const sf::Font& font) {
    FloatingText ft{
        .text = sf::Text(font, sf::String(str), 24),
        .position = pos,
        .color = color,
        .lifetime = 1.0f
    };

    ft.text.setFillColor(color);
    ft.text.setOutlineColor(sf::Color::Black);
    ft.text.setOutlineThickness(2.f);

    // apply offset
    float offsetX = static_cast<float>((rand() % 40) - 20);
    ft.position += sf::Vector2f(offsetX, -20.f);

    floatingTexts.push_back(std::move(ft));
}

void UIRenderer::updateAndDrawFCT(sf::RenderWindow& window, float dt) {
    for (auto it = floatingTexts.begin(); it != floatingTexts.end(); ) {
        it->lifetime -= dt;
        it->position.y -= 60.f * dt; 

        sf::Color c = it->color;
        float alpha = std::max(0.f, it->lifetime);
        c.a = static_cast<uint8_t>(255 * alpha);

        it->text.setFillColor(c);
        
        sf::Color outline = sf::Color::Black;
        outline.a = c.a;
        it->text.setOutlineColor(outline);

        it->text.setPosition(it->position);

        window.draw(it->text);

        if (it->lifetime <= 0) it = floatingTexts.erase(it);
        else ++it;
    }
}

void UIRenderer::drawTooltip(sf::RenderWindow& window, const sf::Font& font, const Card& card, float mouseX, float mouseY) {
    std::ostringstream oss;
    oss << card.name << "\n";
    oss << "Cost: " << card.cost << " | Value: " << card.value;
    // if you are corrupted show the corrupted value
    if (card.corruptedValue > 0) {
        oss << " (+" << card.corruptedValue << ")";
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

void UIRenderer::drawStatusTooltip(sf::RenderWindow& window, const sf::Font& font, const Status& status, sf::Vector2f mousePos) {
    // works almost the same as card tooltip
    std::ostringstream oss;
    oss << status.name << "\n";
    oss << "Duration: " << status.duration << "\n";
    oss << "Intensity: " << status.intensity << "\n";
    oss << "--------------------\n";
    oss << status.getDescription();

    sf::Text text(font, sf::String(oss.str()), 14);
    text.setFillColor(sf::Color::White);

    sf::FloatRect bounds = text.getGlobalBounds();
    sf::RectangleShape bg(sf::Vector2f(bounds.size.x + 20.f, bounds.size.y + 20.f));

    sf::Vector2f pos = mousePos + sf::Vector2f(15.f, 15.f);
    if (pos.x + bg.getSize().x > window.getSize().x) pos.x = mousePos.x - bg.getSize().x - 15.f;
    if (pos.y + bg.getSize().y > window.getSize().y) pos.y = mousePos.y - bg.getSize().y - 15.f;

    bg.setPosition(pos);
    bg.setFillColor(sf::Color(30, 30, 30, 245));
    bg.setOutlineColor(sf::Color::White);
    bg.setOutlineThickness(1.f);

    text.setPosition(pos + sf::Vector2f(10.f, 10.f));

    window.draw(bg);
    window.draw(text);
}

void UIRenderer::loadStatusTextures() {
    std::string pathBleed = "../assets/statusIcons/BleedIcon.png";
    if (!statusTextures[StatusType::Bleed].loadFromFile(pathBleed)) {
    }
    std::string pathDefenceDown = "../assets/statusIcons/DefenceDownIcon.png";
    if (!statusTextures[StatusType::DefenceDown].loadFromFile(pathDefenceDown)) {
    }
    std::string pathDefenceUp = "../assets/statusIcons/DefenceUpIcon.png";
    if (!statusTextures[StatusType::DefenceUp].loadFromFile(pathDefenceUp)) {
    }
    std::string pathDamageUp = "../assets/statusIcons/StrenghtenIcon.png";
    if (!statusTextures[StatusType::DamageUp].loadFromFile(pathDamageUp)) {
    }
    std::string pathDamageDown = "../assets/statusIcons/WeakenIcon.png";
    if (!statusTextures[StatusType::DamageDown].loadFromFile(pathDamageDown)) {
    }
    std::string pathStun = "../assets/statusIcons/StunnedIcon.png";
    if (!statusTextures[StatusType::Stun].loadFromFile(pathStun)) {
    }
    std::string pathVoidMark = "../assets/statusIcons/VoidMarkIcon.png";
    if (!statusTextures[StatusType::VoidMark].loadFromFile(pathVoidMark)) {
    }
}

void UIRenderer::resetHPTracking() {
    lastPlayerHP = -1;
    lastEnemyHP = -1;
}

sf::FloatRect UIRenderer::getEndTurnBounds() const {
    return endTurnBounds;
}

sf::FloatRect UIRenderer::getTransformBounds() const {
    return transformBounds;
}
