#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "Managers/CardResolver.h"
#include "Managers/EnemyFactory.h"
#include "Deck/Deck.h"
#include "Other/CombatState.h"
#include "UI/UIRenderer.h"
#include "Enemy/Enemy.h"

enum class GameState {
    StartScreen,
    Combat,
    Victory,
    Looting,
    GameOver
};

enum class TurnState {
    PlayerTurn,
    EnemyTurn
};

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800u, 800u}),
        "Iliop",
        sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);

    if (!window.isOpen()) {
        std::cerr << "Window failed to open!\n";
        return 1;
    }

    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("../assets/fonts/Knewave.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    CombatState playerState{
        {30,30},    // hp
        {0,10},     // shield
        {3,3},      // mana
        {0,10}      // corruption
    };

    GameState gameState = GameState::StartScreen;
    TurnState turnState = TurnState::PlayerTurn;

    UIRenderer ui;
    Deck deck;
    deck.shuffle();
 
    deck.drawCard(4);

    int currentFloor = 1;

    Enemy enemy = EnemyFactory::create(currentFloor);

    while (window.isOpen()) {

        while (auto eventOpt = window.pollEvent()) {
            if (!eventOpt.has_value()) continue;
            sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>())
                window.close();

            if (auto resized = event.getIf<sf::Event::Resized>()) {
                window.setView(sf::View(sf::FloatRect(
                    {0.f, 0.f},
                    {static_cast<float>(resized->size.x),
                     static_cast<float>(resized->size.y)}
                )));
            }

            if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button != sf::Mouse::Button::Left)
                    continue;

                sf::Vector2f mousePos =
                    window.mapPixelToCoords(
                        sf::Mouse::getPosition(window));

                // start screen
                if (gameState == GameState::StartScreen) {

                    sf::FloatRect startButton(
                        sf::Vector2f(
                            window.getSize().x / 2.f - 100.f,
                            window.getSize().y / 2.f),
                        sf::Vector2f(200.f, 60.f));

                    if (startButton.contains(mousePos)) {
                        gameState = GameState::Combat;
                        turnState = TurnState::PlayerTurn;

                        playerState.mana.current = playerState.mana.max;
                        deck.shuffle();
                    }
                }

                // combat
                else if (gameState == GameState::Combat) {

                    enemy.rollIntent();

                    if (turnState == TurnState::PlayerTurn) {
                        
                        // end Turn button
                        if (ui.getEndTurnBounds().contains(mousePos)) {

                            turnState = TurnState::EnemyTurn;
                        }
                        else {
                            // card clicks
                            for (Card& card : deck.getHand()) {
                                if (card.contains(mousePos.x, mousePos.y)) {
                                    if (CardResolver::play(
                                            card,
                                            playerState,
                                            enemy.getState()))
                                    {
                                        deck.discardCard(card);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                // looting screen
                else if (gameState == GameState::Looting) {

                    sf::FloatRect continueButton(
                        sf::Vector2f(
                            window.getSize().x / 2.f - 100.f,
                            window.getSize().y / 2.f),
                        sf::Vector2f(200.f, 60.f));

                    if (continueButton.contains(mousePos)) {

                        // reset combat state
                        turnState = TurnState::PlayerTurn;

                        deck.shuffle();
                        deck.discardHand();
                        deck.drawCard(4);

                        playerState.mana.current = playerState.mana.max;
                        playerState.corruption.current = 0;

                        enemy = EnemyFactory::create(currentFloor);

                        gameState = GameState::Combat;
                    }
                }
            }
        }

        // enemy turn
        if (gameState == GameState::Combat &&
            turnState == TurnState::EnemyTurn)
        {
            enemy.playTurn(playerState);

            turnState = TurnState::PlayerTurn;

            deck.discardHand();
            deck.drawCard(4);
            playerState.mana.current = playerState.mana.max;
            playerState.corruption.current = 0;
        }

        // check for victory / loss
        if (gameState == GameState::Combat) {
            if (playerState.hp.current <= 0) {
                gameState = GameState::GameOver;
            }

            if (enemy.getState().hp.current <= 0) {
                gameState = GameState::Victory;
            }
        }

        if (gameState == GameState::Victory) {
            currentFloor++;

            // al last set gamestate to looting
            gameState = GameState::Looting;
        }


        // render
        window.clear(sf::Color(40, 40, 40));

        int winW = window.getSize().x;
        int winH = window.getSize().y;

        // render startscreen
        if (gameState == GameState::StartScreen) {

            sf::RectangleShape startButton({200.f, 60.f});
            startButton.setPosition({winW / 2.f - 100.f, winH / 2.f});
            startButton.setFillColor(sf::Color(70,70,70));
            startButton.setOutlineColor(sf::Color::Black);
            startButton.setOutlineThickness(3.f);
            window.draw(startButton);

            sf::Text start(font, "Start", 30);
            start.setPosition({winW / 2.f - 40.f, winH / 2.f + 10.f});
            start.setFillColor(sf::Color::White);
            window.draw(start);
        }
        // render combat screen
        else if (gameState == GameState::Combat) {

            ui.render(window,
                playerState,
                enemy.getState(),
                winW, winH,
                font
            );

            deck.render(window, winW, winH, font);
            enemy.render(window, winW, winH);
        }
        // render game over screen
        else if (gameState == GameState::GameOver) {

            sf::Text over(font, "Game Over", 60);
            over.setPosition({winW / 2.f - 120.f, winH / 2.f});
            over.setFillColor(sf::Color::Red);
            window.draw(over);
        }
        // redner looting screen
        else if (gameState == GameState::Looting) {

            sf::Text victory(font, "Victory!", 50);
            victory.setPosition({winW / 2.f - 110.f, winH / 2.f - 100.f});
            victory.setFillColor(sf::Color::Yellow);
            window.draw(victory);

            sf::RectangleShape continueButton({200.f, 60.f});
            continueButton.setPosition({winW / 2.f - 100.f, winH / 2.f});
            continueButton.setFillColor(sf::Color(70,70,70));
            continueButton.setOutlineColor(sf::Color::Black);
            continueButton.setOutlineThickness(3.f);
            window.draw(continueButton);

            sf::Text cont(font, "Continue", 30);
            cont.setPosition({winW / 2.f - 80.f, winH / 2.f + 10.f});
            cont.setFillColor(sf::Color::White);
            window.draw(cont);
        }
        

        window.display();
    }

    return 0;
}
