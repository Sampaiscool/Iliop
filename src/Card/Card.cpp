#include "Card.h"

void Card::draw(sf::RenderWindow& window, const sf::Font& font) const {
    sf::RectangleShape rect(sf::Vector2f(static_cast<float>(w),
                                         static_cast<float>(h)));
    rect.setPosition(sf::Vector2f(static_cast<float>(x),
                                  static_cast<float>(y)));

    switch (type) {
        case CardType::Damage: rect.setFillColor(sf::Color(200, 50, 50)); break;
        case CardType::Heal:   rect.setFillColor(sf::Color(50, 200, 50)); break;
        case CardType::Shield: rect.setFillColor(sf::Color(50, 50, 200)); break;
    }

    window.draw(rect);

    sf::Text nameText(font, sf::String(name), static_cast<unsigned int>(h / 6));
    nameText.setPosition(sf::Vector2f(static_cast<float>(x + 5),
                                      static_cast<float>(y + 5)));
    nameText.setFillColor(sf::Color::Black);
    window.draw(nameText);

    sf::Text valueText(font, sf::String(std::to_string(value)),
                       static_cast<unsigned int>(h / 6));
    valueText.setPosition(sf::Vector2f(static_cast<float>(x + 5),
                                       static_cast<float>(y + h - h / 4)));
    valueText.setFillColor(sf::Color::Red);
    window.draw(valueText);
}

