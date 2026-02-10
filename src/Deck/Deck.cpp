#include "Deck.h"
#include <algorithm>
#include <random>

int deckCardAmount = 40;

Deck::Deck() {
    for (int i = 0; i < deckCardAmount; ++i) {
        Card card;
        card.type = static_cast<CardType>(i % 3);
        switch (card.type) {
            case CardType::Damage:
                card.name = "Magic Blast";
                card.effect = CardEffect::DealDamage;
                break;
            case CardType::Heal:
                card.name = "Healing Winds";
                card.effect = CardEffect::Heal;
                break;
            case CardType::Shield:
                card.name = "Protecting Barrier";
                card.effect = CardEffect::GainShield;
                break;
        }
        card.value = 5 + (i % 5);
        drawPile.push_back(card);
    }
}

void Deck::render(sf::RenderWindow& window, int winW, int winH, const sf::Font& font) {
    int cardW = winW / 10;
    int cardH = winH / 6;

    int handAreaW = winW * 0.6f;
    int handStartX = 40;
    int handY = winH - cardH - 40;

    int spacing = handAreaW / (hand.size() + 1);

    for (size_t i = 0; i < hand.size(); ++i) {
        Card& card = hand[i];

        card.x = handStartX + spacing * (i + 1) - cardW / 2;
        card.y = handY;
        card.w = cardW;
        card.h = cardH;

        // Draw rectangle
        sf::RectangleShape rect(sf::Vector2f(cardW, cardH));
        rect.setPosition(static_cast<float>(card.x), static_cast<float>(card.y));
        rect.setFillColor(sf::Color(200, 200, 200));
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2.f);

        window.draw(rect);

        // Draw card name
        sf::Text text;
        text.setFont(font);
        text.setString(card.name);
        text.setCharacterSize(cardH / 5); // scale with card height
        text.setFillColor(sf::Color::Black);
        text.setPosition(
            static_cast<float>(card.x + 5),
            static_cast<float>(card.y + 5)
        );

        window.draw(text);

        // Draw card value
        sf::Text valueText;
        valueText.setFont(font);
        valueText.setString(std::to_string(card.value));
        valueText.setCharacterSize(cardH / 6);
        valueText.setFillColor(sf::Color::Red);
        valueText.setPosition(
            static_cast<float>(card.x + 5),
            static_cast<float>(card.y + cardH - cardH / 4)
        );

        window.draw(valueText);
    }
}

std::vector<Card>& Deck::getHand() {
    return hand;
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(drawPile.begin(), drawPile.end(), g);
}

void Deck::drawCard(int amount) {
    for (int i = 0; i < amount && !drawPile.empty(); ++i) {
        hand.push_back(drawPile.back());
        drawPile.pop_back();
    }
}

void Deck::discardCard(const Card& card) {
    auto it = std::find_if(hand.begin(), hand.end(),
        [&](const Card& c) { return &c == &card; });

    if (it != hand.end()) {
        discardPile.push_back(*it);
        hand.erase(it);
    }
}

