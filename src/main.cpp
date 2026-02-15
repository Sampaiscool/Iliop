#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>

#include "Managers/CardResolver.h"
#include "Managers/EnemyFactory.h"
#include "Managers/CharacterFactory.h"
#include "Deck/Deck.h"
#include "Other/CombatState.h"
#include "Other/Particle.h"
#include "UI/UIRenderer.h"
#include "Enemy/Enemy.h"
#include "Character/Character.h"

enum class GameState {
    StartScreen,
    CharacterSelect,
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

    GameState gameState = GameState::StartScreen;
    TurnState turnState = TurnState::PlayerTurn;

    CombatState playerState;
    Deck deck;

    UIRenderer ui;

    sf::Clock particleClock;
    ParticleSystem corruptionParticles;

    int currentFloor = 1;

    Enemy enemy = EnemyFactory::create(currentFloor);

    while (window.isOpen()) {
        float dt = particleClock.restart().asSeconds();

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
                float scale = resized->size.x / 800.f; // 800 is base width
                    corruptionParticles.setScale(scale);
            }

            // mouse wordt geklikt
            if (auto mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button != sf::Mouse::Button::Left)
                    continue;

                sf::Vector2f mousePos =
                    window.mapPixelToCoords(
                        sf::Mouse::getPosition(window));

                // checkt of je klikt op de start knop
                if (gameState == GameState::StartScreen) {

                    sf::FloatRect startButton(
                        sf::Vector2f(
                            window.getSize().x / 2.f - 100.f,
                            window.getSize().y / 2.f),
                        sf::Vector2f(200.f, 60.f));

                    if (startButton.contains(mousePos)) {
                        gameState = GameState::CharacterSelect;
                        // gameState = GameState::Combat;
                        // turnState = TurnState::PlayerTurn;
                        //
                        // playerState.mana.current = playerState.mana.max;
                        // deck.shuffle();
                    }
                }
                else if (gameState == GameState::CharacterSelect) {
                    int winW = window.getSize().x;
                    int winH = window.getSize().y;

                    float buttonWidth = 220.f;
                    float buttonHeight = 70.f;
                    float centerX = winW / 2.f - buttonWidth / 2.f;
                    float startY = winH / 2.f - 120.f;
                    float spacing = 100.f;

                    std::vector<CharacterClass> classList = {
                        CharacterClass::Mage,
                        CharacterClass::Warrior,
                        CharacterClass::Cleric
                    };

                    for (int i = 0; i < classList.size(); ++i) {

                        sf::FloatRect bounds(
                            sf::Vector2f(centerX, startY + i * spacing),
                            sf::Vector2f(buttonWidth, buttonHeight)
                        );

                        if (bounds.contains(mousePos)) {

                            Character character = CharacterFactory::create(classList[i]);

                            playerState = character.baseStats;

                            deck.setDeck(std::move(character.starterDeck));
                            deck.shuffle();
                            deck.drawCard(4);

                            gameState = GameState::Combat;
                            break;
                        }
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
        else if (gameState == GameState::CharacterSelect) {
            float buttonWidth = 220.f;
            float buttonHeight = 70.f;
            float centerX = winW / 2.f - buttonWidth / 2.f;
            float startY = winH / 2.f - 120.f;
            float spacing = 100.f;

            std::vector<std::pair<std::string, CharacterClass>> classes = {
                {"Mage", CharacterClass::Mage},
                {"Warrior", CharacterClass::Warrior},
                {"Cleric", CharacterClass::Cleric}
            };

            for (int i = 0; i < classes.size(); ++i) {

                sf::RectangleShape button({buttonWidth, buttonHeight});
                button.setPosition({centerX, startY + i * spacing});
                button.setFillColor(sf::Color(60,60,80));
                button.setOutlineColor(sf::Color::Black);
                button.setOutlineThickness(3.f);
                window.draw(button);

                sf::Text text(font, classes[i].first, 28);
                text.setPosition({centerX + 40.f, startY + i * spacing + 18.f});
                text.setFillColor(sf::Color::White);
                window.draw(text);
            }
        }
        // render combat screen
        else if (gameState == GameState::Combat) {

            ui.render(window,
                playerState,
                enemy.getState(),
                winW, winH,
                font
            );

            corruptionParticles.update(dt);

            corruptionParticles.draw(window);

            deck.render(window, winW, winH, font, playerState, corruptionParticles);
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
