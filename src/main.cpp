#include <SFML/Graphics.hpp>
#include <iostream>

#include "Managers/CardResolver.h"
#include "Deck/Deck.h"
#include "Other/CombatState.h"
#include "UI/UIRenderer.h"
#include "Enemy/Enemy.h"

int main() {
    // Create the window
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(800, 800)),
        "Card Engine",
        sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close
    );
    window.setFramerateLimit(60);

    // Load the font
    if (!font.loadFromFile("../assets/fonts/Monsterrat.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }


    // Setup player and enemy
    CombatState playerState{ {30,30}, {0,10}, {3,3} };
    UIRenderer ui;
    Deck deck;
    deck.shuffle();
    deck.drawCard(4);
    Enemy enemy(10, 10, 1);

    // Main loop

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::Resized) {
            sf::View view(sf::FloatRect(
                0.f, 0.f,
                static_cast<float>(event.size.width),
                static_cast<float>(event.size.height)
            ));
            window.setView(view);
        }
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) 
        {
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

    return 0;
}
