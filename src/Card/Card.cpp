#include "Card.h"

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

    switch (type) {
        case CardType::Damage:
          rect.setFillColor(sf::Color(200, 50, 50)); break;
        case CardType::Heal:
          rect.setFillColor(sf::Color(50, 200, 50)); break;
        case CardType::Shield:
          rect.setFillColor(sf::Color(50, 50, 200)); break;
    }

    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(1.f);
    window.draw(rect);

    sf::Text costText(font, sf::String(std::to_string(cost)),
                       static_cast<unsigned int>(h / 6));

    costText.setPosition(sf::Vector2f(static_cast<float>(x + 5),
                                       static_cast<float>(y + h - h / 4)));

    costText.setFillColor(sf::Color::Black);
    window.draw(costText);
}
