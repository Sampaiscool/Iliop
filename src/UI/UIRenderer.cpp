#include "UIRenderer.h"
#include <string>
#include <sstream>
#include <iostream>

/// @brief draws a bar with smfl
/// @param window the window to draw on
/// @param x the x position of the bar
/// @param y the y position of the bar
/// @param w the width of the bar
/// @param h the height of the bar
/// @param current the current value
/// @param max the maximum value
/// @param fill the color of the inside of the bar
/// @param back the color of the inside that shows the missing fill
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

/// @brief draws text with sfml
/// @param window the window to draw on
/// @param font the font to use
/// @param str the string to display
/// @param x the x position of the text
/// @param y the y position of the text
/// @param charSize the size of the characters
/// @param color the color of the text
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

/// @brief gets the associated color for a class
/// @param type the class type to use
/// @return the correct color based on the given class
sf::Color getClassColor(Class type) {
    switch (type) {
        case Class::Mage:    return sf::Color(50, 150, 255); // blue
        case Class::Warrior: return sf::Color(255, 50, 50);  // red
        case Class::Cleric:  return sf::Color(255, 255, 150); // gold/yellow
        default:             return sf::Color::White;
    }
}

/// @brief draws the portrait box for the player
/// @param window the window to draw on
/// @param font what font to use
/// @param player the Character to use
/// @param winW the width of the window
/// @param winH the height of the window
/// @param portraitTextures all the textures of the portraits
void drawPlayerPortrait(sf::RenderWindow& window, const sf::Font& font, const Character& player, int winW, int winH, const std::map<CharacterName, sf::Texture>& portraitTextures) {
    float boxW = winW / 5.f;
    float boxH = winW / 5.f;
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

/// @brief draws all the status icons in a scrollable bar
/// @param window the window to draw on
/// @param font the font to use
/// @param statuses the list of statusses to draw
/// @param x the x position of the bar
/// @param y the y position of the bar
/// @param mousePos the position of the mouse
/// @param textures the textures of the status icons
/// @param scrollOffset the offset for scrolling trough the bar
/// @return a pointer to hovered status, nullptr if none
static const Status* drawStatusIcons(sf::RenderWindow& window, const sf::Font& font,
    const std::vector<std::unique_ptr<Status>>& statuses,
    int x, int y, sf::Vector2f mousePos, const std::map<StatusType, sf::Texture>& textures,
    int scrollOffset = 0)
{
    float iconSize = (window.getSize().x / 20);
    float spacing = iconSize / 4;
    const Status* hoveredStatus = nullptr;
    const int MAX_VISIBLE = 4;

    // draw container background if there are statuses
    if (!statuses.empty()) {
        float containerW = (iconSize + spacing) * MAX_VISIBLE - spacing;
        sf::RectangleShape container(sf::Vector2f(containerW, iconSize + 4.f));
        container.setPosition(sf::Vector2f(static_cast<float>(x) - 2.f, static_cast<float>(y) - 2.f));
        container.setFillColor(sf::Color(30, 30, 30));
        container.setOutlineColor(sf::Color(100, 100, 100));
        container.setOutlineThickness(1.f);
        window.draw(container);
    }

    // clamp scroll offset
    int maxScroll = static_cast<int>(statuses.size()) - MAX_VISIBLE;
    if (maxScroll < 0) maxScroll = 0;
    int clamped = (scrollOffset < 0) ? 0 : ((scrollOffset > maxScroll) ? maxScroll : scrollOffset);

    // draw only MAX_VISIBLE icons starting from clamped scroll offset
    int end = std::min(static_cast<int>(statuses.size()), clamped + MAX_VISIBLE);
    for (int i = clamped; i < end; ++i) {
        int displayPos = i - clamped;
        sf::Vector2f pos(static_cast<float>(x) + (displayPos * (iconSize + spacing)), static_cast<float>(y));
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

/// @brief renders the whole combat UI
/// @param window the window to draw on
/// @param player the player's Character object
/// @param playerState the player's combat state
/// @param enemyState the enemies combat state
/// @param winW the width of the window
/// @param winH the height of the window
/// @param font the font to use
/// @param enemyIntentDescription refrence to the enemies intent
void UIRenderer::render(sf::RenderWindow& window,
    const Character& player,
    const CombatState& playerState,
    const CombatState& enemyState,
    int winW,
    int winH,
    const sf::Font& font,
    const std::string& enemyIntentDescription)
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

    // intent label
    if (!enemyIntentDescription.empty()) {
        int intentY = winW / 4 + (enemyY + (enemyY / 3.5));
        drawText(window, font, enemyIntentDescription,
                 enemyX, intentY,
                 enemyBarHeight * 0.7, sf::Color(255, 200, 50));
    }

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

    // player damage events
    for (const auto& evt : playerState.damageEvents) {
        sf::Color color;
        std::string text;
        if (evt.isHeal) {
            color = sf::Color(100, 255, 100); // Green for heal
            text = "+" + std::to_string(evt.amount);
        } else {
            color = sf::Color(255, 100, 100); // Red for damage
            text = "-" + std::to_string(evt.amount);
        }
        // random offset for each event
        float offsetX = (rand() % 30 - 15) * 1.f;
        float offsetY = (rand() % 20 - 10) * 1.f;
        sf::Vector2f pos = lastPlayerBarPos;
        pos.x += offsetX;
        pos.y += offsetY;

        // text gains +2 size per damage, max 60
        int charSize = std::min(60, 20 + evt.amount * 3);
        sf::Text fctText(font, sf::String(text), static_cast<unsigned int>(charSize));
        fctText.setFillColor(color);
        fctText.setOutlineColor(sf::Color::Black);
        fctText.setOutlineThickness(1.f);

        floatingTexts.push_back({fctText, pos, color, 0.8f});
    }
    playerState.clearDamageEvents();

    // enemy damage events
    for (const auto& evt : enemyState.damageEvents) {
        sf::Color color;
        std::string text;
        if (evt.isHeal) {
            color = sf::Color(100, 255, 100);
            text = "+" + std::to_string(evt.amount);
        } else {
            color = sf::Color(255, 100, 100);
            text = "-" + std::to_string(evt.amount);
        }
        float offsetX = (rand() % 30 - 15) * 1.f;
        float offsetY = (rand() % 20 - 10) * 1.f;
        sf::Vector2f pos = lastEnemyBarPos;
        pos.x += offsetX;
        pos.y += offsetY;

        // Scale size by damage amount
        int charSize = std::min(60, 20 + evt.amount * 3);
        sf::Text fctText(font, sf::String(text), static_cast<unsigned int>(charSize));
        fctText.setFillColor(color);
        fctText.setOutlineColor(sf::Color::Black);
        fctText.setOutlineThickness(1.f);

        floatingTexts.push_back({fctText, pos, color, 0.8f});
    }
    enemyState.clearDamageEvents();

    // Update HP tracking after rendering events
    lastPlayerHP = playerState.hp.current;
    lastEnemyHP = enemyState.hp.current;

    // get thah mouse position
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // player status
    int playerStatusX = x;
    int playerStatusY = corruptionY - barHeight - (winH / 8); 
    const Status* hovered = drawStatusIcons(window, font, playerState.statuses, 
                                           playerStatusX, playerStatusY, mousePos, statusTextures,
                                           playerStatusScroll);

    // enemy status
    int enemyStatusX = enemyX;
    int enemyStatusY = enemyY - (enemyY / 2 - 10);
    const Status* enemyHovered = drawStatusIcons(window, font, enemyState.statuses, 
                                                enemyStatusX, enemyStatusY, mousePos, statusTextures,
                                                enemyStatusScroll);


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

    // if we are hovering draw tooltip last
    if (hovered) {
        drawStatusTooltip(window, font, *hovered, mousePos);
    } else if (enemyHovered) {
        drawStatusTooltip(window, font, *enemyHovered, mousePos);
    }
}

/// @brief spawn floating text of the correct healthbar
/// @param pos the position where the text should spawn
/// @param str the string to display
/// @param color the color of the text
/// @param font the font to use
/// @param charSize the size of the characters
void UIRenderer::spawnFCT(sf::Vector2f pos, std::string str, sf::Color color, const sf::Font& font, int charSize) {
    sf::Text txt(font, sf::String(str), static_cast<unsigned int>(charSize > 0 ? charSize : 24));
    txt.setFillColor(color);
    txt.setOutlineColor(sf::Color::Black);
    txt.setOutlineThickness(2.f);

    // apply offset
    float offsetX = static_cast<float>((rand() % 40) - 20);
    pos += sf::Vector2f(offsetX, -20.f);

    floatingTexts.push_back({txt, pos, color, 1.0f});
}

/// @brief updates and draws floating combat text
/// @param window the window to draw on
/// @param dt the delta time
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

/// @brief draws a tooltip for the hovered card
/// @param window the window to draw on
/// @param font the font to use
/// @param card the card for wich to draw the tooltip of
/// @param mouseX the x position of the mouse
/// @param mouseY the y position of the mouse
void UIRenderer::drawTooltip(sf::RenderWindow& window, const sf::Font& font, const Card& card, float mouseX, float mouseY) {
    std::ostringstream oss;
    oss << card.name << "\n";
    oss << "Cost: " << card.cost << " | Value: " << card.value;
    // if corrupted show the corrupted value
    if (card.corruptedValue > 0) {
        oss << " (+" << card.corruptedValue << ")";
    }

    oss << "\n------------------\n" 
        << card.description;
    
    // show orb imprints
    if (card.bonusValue > 0 || card.costReduction > 0 || card.drawOnUse > 0 || card.replayCount > 0 || card.freeOnceCount > 0 || card.applyDamageUp > 0 || card.applyDefenceUp > 0 || card.applyRegen > 0 || card.applyBleed > 0 || card.healOnUse > 0 || card.shieldOnUse > 0) {
        oss << "\n------------------\n[IMPRINTS]";
        if (card.bonusValue > 0) {
            oss << "\n+" << card.bonusValue << " Value";
        }
        if (card.costReduction > 0) {
            oss << "\n-" << card.costReduction << " Cost";
        }
        if (card.drawOnUse > 0) {
            oss << "\nDraw " << card.drawOnUse;
        }
        if (card.replayCount > 0) {
            oss << "\nReplay x" << card.replayCount;
        }
        if (card.freeOnceCount > 0) {
            oss << "\nFree " << card.freeOnceCount << "x";
        }
        if (card.applyDamageUp > 0) {
            oss << "\n+Damage Up x" << card.applyDamageUp;
        }
        if (card.applyDefenceUp > 0) {
            oss << "\n+Defence Up x" << card.applyDefenceUp;
        }
        if (card.applyRegen > 0) {
            oss << "\n+Regen x" << card.applyRegen;
        }
        if (card.applyBleed > 0) {
            oss << "\n+Bleed x" << card.applyBleed;
        }
        if (card.healOnUse > 0) {
            oss << "\nHeal " << card.healOnUse;
        }
        if (card.shieldOnUse > 0) {
            oss << "\nShield " << card.shieldOnUse;
        }
    }

    sf::Text descText(font, sf::String(oss.str()), (window.getSize().x / 40));
    descText.setFillColor(sf::Color::White);

    // get the background and shape
    sf::FloatRect bounds = descText.getLocalBounds();
    sf::Vector2f boxSize(bounds.size.x + 20.f, bounds.size.y + 25.f);
    sf::RectangleShape bg(boxSize);

    sf::Vector2f position(mouseX + 15.f, mouseY + 15.f);

    // this way the tooltip cant get out of bounds
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

/// @brief draws a tooltip of hovered status
/// @param window the window to draw on
/// @param font the font to use
/// @param status the status for wich to draw the tooltip of
/// @param mousePos the position of the mouse
void UIRenderer::drawStatusTooltip(sf::RenderWindow& window, const sf::Font& font, const Status& status, sf::Vector2f mousePos) {
    // works almost the same as card tooltip
    std::ostringstream oss;
    oss << status.name << "\n";
    oss << "Duration: " << status.duration << "\n";
    oss << "Intensity: " << status.intensity << "\n";
    oss << "--------------------\n";
    oss << status.getDescription();

    sf::Text text(font, sf::String(oss.str()), (window.getSize().x / 45));
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

/// @brief loads all the status textures
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
    std::string pathBlessed = "../assets/statusIcons/BlessedIcon.png";
    if (!statusTextures[StatusType::Blessed].loadFromFile(pathBlessed)) {
    }
    std::string pathLocked = "../assets/statusIcons/LockedIcon.png";
    if (!statusTextures[StatusType::Locked].loadFromFile(pathLocked)) {
    }
    std::string pathRage = "../assets/statusIcons/RageIcon.png";
    if (!statusTextures[StatusType::Rage].loadFromFile(pathRage)) {
    }
}

/// @brief resets the HP tracking
// BUG: this isn't working, HP still shows after a new enemy is spawned
void UIRenderer::resetHPTracking() {
    lastPlayerHP = -1;
    lastEnemyHP = -1;
}

/// @brief gets the bounds of the end turn button
/// @return the bounds of the end turn button
sf::FloatRect UIRenderer::getEndTurnBounds() const {
    return endTurnBounds;
}

/// @brief gets the bounds of the transform button
/// @return the bounds of the transform button
sf::FloatRect UIRenderer::getTransformBounds() const {
    return transformBounds;
}

/// @brief scrolls trough the players statusses
/// @param delta the amount to scroll
void UIRenderer::scrollPlayerStatuses(int delta) {
    playerStatusScroll += delta;
}

/// @brief scrolls through the enemies statusses
/// @param delta the amount to scroll
void UIRenderer::scrollEnemyStatuses(int delta) {
    enemyStatusScroll += delta;
}

/// @brief resets the scroll ofsets in both statues bars
void UIRenderer::resetStatusScrolls() {
    playerStatusScroll = 0;
    enemyStatusScroll = 0;
}
