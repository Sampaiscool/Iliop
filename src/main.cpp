#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>

#include "Managers/CardResolver.h"
#include "Deck/Deck.h"
#include "Other/CombatState.h"
#include "UI/UIRenderer.h"
#include "Enemy/Enemy.h"

int main() {

    // startup the window
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u{800, 800}),
        "Card Engine",
        sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close
    );

    window.setFramerateLimit(60);

    // load the font
    sf::Font font;
    if (!font.openFromFile("assets/fonts/Monsterrat.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    CombatState playerState{
        {30, 30}, // hp
        {0, 10},  // shield
        {3, 3}    // mana
    };

    UIRenderer ui;
    Deck deck;

    deck.shuffle();
    deck.drawCard(4);

    Enemy enemy(10, 10, 1);

    bool running = true;

    // main gameplay loop:
    while (window.isOpen()) {

        // events
        while (auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (auto resized = event->getIf<sf::Event::Resized>()) {
                sf::View view(sf::FloatRect(
                    {0.f, 0.f},
                    {static_cast<float>(resized->size.x),
                     static_cast<float>(resized->size.y)}
                ));
                window.setView(view);
            }

            if (auto mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left) {

                    sf::Vector2i mousePos =
                        sf::Mouse::getPosition(window);

                    for (Card& card : deck.getHand()) {
                        if (card.contains(mousePos.x, mousePos.y)) {
                            CardResolver::play(
                                card,
                                playerState,
                                enemy.getState()
                            );
                            deck.discardCard(card);
                            break;
                        }
                    }
                }
            }
        }

        // draw the cards
        window.clear(sf::Color(40, 40, 40));

        sf::Vector2u size = window.getSize();
        int winW = size.x;
        int winH = size.y;

        ui.render(window, playerState, winW, winH, font);
        deck.render(window, winW, winH, font);
        enemy.render(window, winW, winH);

        window.display();
    }

    return 0;
}

