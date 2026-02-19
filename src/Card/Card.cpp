#include "Card.h"

static sf::Color themeToColor(CardTheme theme) {
    switch(theme) {
        case CardTheme::Red:    return sf::Color(200, 50, 50);
        case CardTheme::Blue:   return sf::Color(50, 50, 200);
        case CardTheme::Green:  return sf::Color(50, 180, 50);
        case CardTheme::Purple: return sf::Color(140, 50, 200);
        case CardTheme::Gold:   return sf::Color(224, 198, 68);
        default:                return sf::Color(100, 100, 100);
    }
}

void Card::draw(sf::RenderWindow& window, const sf::Font& font, bool isCorrupted, const CombatState& playerState) const {
    sf::RectangleShape rect(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
    rect.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    sf::Color classColor;

    sf::Color corruptionPurple = sf::Color(118, 50, 121);
    sf::Color transformYellow = sf::Color(224, 198, 68);

    if (playerState.corruption.current >= playerState.transformThreshold) {
        sf::RectangleShape outerBorder = rect;
        outerBorder.setOutlineColor(corruptionPurple);
        outerBorder.setOutlineThickness(4.f);
        outerBorder.setFillColor(sf::Color::Transparent);
        window.draw(outerBorder);
    }

    if (playerState.isTransformed) {
        sf::RectangleShape innerBorder = rect;
        innerBorder.setOutlineColor(transformYellow);
        innerBorder.setOutlineThickness(2.f);
        innerBorder.setFillColor(sf::Color::Transparent);
        window.draw(innerBorder);
    }

   rect.setFillColor(themeToColor(this->theme));

    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(1.f);
    window.draw(rect);

    sf::Text costText(font, sf::String(std::to_string(cost)),
                       static_cast<unsigned int>(h / 6));

    costText.setPosition(sf::Vector2f(static_cast<float>(x + 5),
                                       static_cast<float>(y + h - h / 4)));

    costText.setFillColor(sf::Color::Black);
    window.draw(costText);

    sf::Text nameText(font, name, static_cast<unsigned int>(h / 10));
    nameText.setPosition({(float)x + 5, (float)y + 5});
    nameText.setFillColor(sf::Color::White);
    window.draw(nameText);
}
