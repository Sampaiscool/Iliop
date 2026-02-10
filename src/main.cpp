#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "Managers/CardResolver.h"
#include "Deck/Deck.h"
#include "Other/CombatState.h"
#include "UI/UIRenderer.h"
#include "Enemy/Enemy.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800u, 800u}),
        "Card Engine",
        sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);

    if (!window.isOpen()) {
        std::cerr << "Window failed to open!\n";
        return 1;
    }

    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("../assets/fonts/Monsterrat.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    CombatState playerState{{30,30}, {0,10}, {3,3}};
    UIRenderer ui;
    Deck deck;
    deck.shuffle();
    deck.drawCard(4);
    Enemy enemy(10, 10, 1);

    while (window.isOpen()) {
        while (auto eventOpt = window.pollEvent()) {
            if (!eventOpt.has_value()) continue;
            sf::Event& event = *eventOpt;

            // Check for Closed event
            if (auto closed = event.getIf<sf::Event::Closed>()) {
                window.close();
            }

            if (auto resized = event.getIf<sf::Event::Resized>()) {
                window.setView(sf::View(sf::FloatRect(
                {0.f, 0.f},
                {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)}
                )));
            }


            // Check for Mouse Button Pressed
            if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (Card& card : deck.getHand()) {
                        if (card.contains(mousePos.x, mousePos.y)) {
                            CardResolver::play(card, playerState, enemy.getState());
                            deck.discardCard(card);
                            break;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(40, 40, 40));
        int winW = window.getSize().x;
        int winH = window.getSize().y;

        ui.render(window, playerState, winW, winH, font);
        deck.render(window, winW, winH, font);
        enemy.render(window, winW, winH);

        window.display();
    }

    return 0;
}


