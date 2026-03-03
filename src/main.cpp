#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <algorithm>
#include <vector>

#include "Managers/CardResolver.h"
#include "Managers/EnemyFactory.h"
#include "Managers/CharacterFactory.h"
#include "Managers/CardFactory.h"
#include "Deck/Deck.h"
#include "Other/GameState.h"
#include "Other/CombatState.h"
#include "Other/Particle.h"
#include "UI/UIRenderer.h"
#include "Enemy/Enemy.h"
#include "Character/Character.h"

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({800u, 800u}),
        "Iliop",
        sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);

    if (!window.isOpen()) {
        std::cerr << "Window failed to open!\n";
        return 1;
    }

    window.setFramerateLimit(30);

    sf::Font font;
    if (!font.openFromFile("../assets/fonts/Knewave.ttf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    GameState gameState = GameState::StartScreen;
    TurnState turnState = TurnState::PlayerTurn;

    Class selectedClass;
    std::optional<Character> player;

    CombatState playerState;
    Deck deck;

    std::vector<Card> lootOptions;
    std::vector<size_t> forgeSelectedIndices;

    UIRenderer ui;
    ui.loadPlayerTextures();
    ui.loadStatusTextures();

    sf::Clock particleClock;
    ParticleSystem corruptionParticles;

    int currentFloor = 1;

    Enemy enemy = EnemyFactory::create(currentFloor);
    // first intent roll so the UI shows something before the enemy acts
    enemy.rollIntent();

    while (window.isOpen()) {
        float dt = particleClock.restart().asSeconds();

        float winW = static_cast<float>(window.getSize().x);
        float winH = static_cast<float>(window.getSize().y);

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
                float scale = resized->size.x / 800.f;
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
                        gameState = GameState::ClassSelect;
                    }
                }
                else if (gameState == GameState::ClassSelect) {
                    int winW = window.getSize().x;
                    int winH = window.getSize().y;

                    float buttonWidth = 220.f;
                    float buttonHeight = 70.f;
                    float centerX = winW / 2.f - buttonWidth / 2.f;
                    float startY = winH / 2.f - 120.f;
                    float spacing = 100.f;

                    std::vector<Class> classList = {
                        Class::Warrior,
                        Class::Cleric,
                        Class::Mage,
                        Class::Ranger
                    };

                    for (int i = 0; i < classList.size(); ++i) {

                        sf::FloatRect bounds(
                            sf::Vector2f(centerX, startY + i * spacing),
                            sf::Vector2f(buttonWidth, buttonHeight)
                        );

                        if (bounds.contains(mousePos)) {
                            selectedClass = classList[i];
                            gameState = GameState::CharacterSelect;
                            break;
                        }
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

                    std::vector<CharacterName> charactersList = {
                        CharacterName::Hiroshi,
                        CharacterName::Phlox,
                        CharacterName::Vortex,
                        CharacterName::MathewsLift
                    };

                    for (int i = 0; i < charactersList.size(); ++i) {

                        sf::FloatRect bounds(
                            sf::Vector2f(centerX, startY + i * spacing),
                            sf::Vector2f(buttonWidth, buttonHeight)
                        );

                        if (bounds.contains(mousePos)) {

                            Character character = CharacterFactory::create(selectedClass, charactersList[i]);

                            playerState = std::move(character.baseStats);
                            deck.setDeck(std::move(character.starterDeck));
                            deck.shuffle();
                            deck.drawCard(4);

                            player.emplace(std::move(character));

                            gameState = GameState::Combat;
                            break;
                        }
                    }
                }
                // combat
                else if (gameState == GameState::Combat) {

                    // removed rolling every click; intent is handled on turn boundaries
                    if (turnState == TurnState::PlayerTurn) {
                        // end turn button
                        if (ui.getEndTurnBounds().contains(mousePos)) {

                            playerState.endTurn(playerState);

                            turnState = TurnState::EnemyTurn;
                        }
                        // transform button
                        else if (ui.getTransformBounds().contains(mousePos) && playerState.mana.current == playerState.mana.max) {
                            if (playerState.isTransformed != true) {
                                playerState.mana.current -= playerState.mana.max;
                                playerState.transform(enemy.getState());
                            }
                        }
                        else {
                            // card clicks
                            for (Card& card : deck.getHand()) {
                                if (card.contains(mousePos.x, mousePos.y)) {
                                    if (CardResolver::play(
                                            card,
                                            playerState,
                                            enemy.getState(),
                                            deck))
                                    {
                                        deck.discardCard(card);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                // looting screen:
                else if (gameState == GameState::Looting) {
                    sf::FloatRect continueButton(
                        sf::Vector2f(
                            window.getSize().x / 2.f - 100.f,
                            window.getSize().y / 2.f),
                        sf::Vector2f(200.f, 60.f));

                    for (size_t i = 0; i < lootOptions.size(); ++i) {
                        sf::FloatRect cardRect({(float)lootOptions[i].x, (float)lootOptions[i].y},
                                              {(float)lootOptions[i].w, (float)lootOptions[i].h});

                        if (cardRect.contains(mousePos)) {
                            // capture name before moving out of the vector slot
                            std::string chosenName = lootOptions[i].name;
                            deck.addCardToPermanentCollection(std::move(lootOptions[i]));
                            // also give a fresh copy to the draw pile so you can draw it
                            deck.addCardToDrawPile(CardFactory::create(chosenName));
                            lootOptions.clear();
                            break;
                        }
                    }
                    if (continueButton.contains(mousePos)) {

                        // reset combat state
                        turnState = TurnState::PlayerTurn;

                        deck.shuffle();
                        deck.discardHand();
                        deck.drawCard(4);

                        playerState.mana.current = playerState.mana.max;
                        playerState.corruption.current = 0;

                        playerState.transformTime = 0;
                        playerState.isTransformed = false;

                        enemy = EnemyFactory::create(currentFloor);
                        enemy.rollIntent(); // prepare the new enemy's first intent

                        ui.resetHPTracking();

                        // Always go to forge after looting!
                        gameState = GameState::Forge;
                    }
                }
                // FORGE - combine two cards!
                else if (gameState == GameState::Forge) {
                    sf::FloatRect continueButton(
                        sf::Vector2f(window.getSize().x / 2.f - 100.f, window.getSize().y * 0.85f),
                        sf::Vector2f(200.f, 60.f));
                    
                    // Get all cards from permanent collection
                    auto& allCards = deck.getPermanentCollection();
                    
                    for (size_t i = 0; i < allCards.size(); ++i) {
                        sf::FloatRect cardRect({(float)allCards[i].x, (float)allCards[i].y},
                                              {(float)allCards[i].w, (float)allCards[i].h});
                        
                        if (cardRect.contains(mousePos)) {
                            // Check if already selected
                            auto it = std::find(forgeSelectedIndices.begin(), forgeSelectedIndices.end(), i);
                            if (it != forgeSelectedIndices.end()) {
                                forgeSelectedIndices.erase(it);
                            } else if (forgeSelectedIndices.size() < 2) {
                                forgeSelectedIndices.push_back(i);
                                
                                // If 2 selected, create a fusion!
                                if (forgeSelectedIndices.size() == 2) {
                                    // Sort indices descending so we can remove from back first
                                    std::sort(forgeSelectedIndices.begin(), forgeSelectedIndices.end(), std::greater<size_t>());
                                    
                                    // Check if BOTH cards are fusions - DOUBLE FUSION!
                                    bool isDoubleFusion = (allCards[forgeSelectedIndices[0]].name == "Divine Arrow" ||
                                                           allCards[forgeSelectedIndices[0]].name == "Void Storm" ||
                                                           allCards[forgeSelectedIndices[0]].name == "Beast Rampage" ||
                                                           allCards[forgeSelectedIndices[0]].name == "Cosmic Shield" ||
                                                           allCards[forgeSelectedIndices[0]].name == "Blood Frenzy") &&
                                                          (allCards[forgeSelectedIndices[1]].name == "Divine Arrow" ||
                                                           allCards[forgeSelectedIndices[1]].name == "Void Storm" ||
                                                           allCards[forgeSelectedIndices[1]].name == "Beast Rampage" ||
                                                           allCards[forgeSelectedIndices[1]].name == "Cosmic Shield" ||
                                                           allCards[forgeSelectedIndices[1]].name == "Blood Frenzy");
                                    
                                    std::string fusionName;
                                    if (isDoubleFusion) {
                                        // Double fusion - SUPER WACKY!
                                        std::vector<std::string> doubleFusionCards = {"Omega Annihilation", "Universal Singularity", "Primordial Chaos", "Existential Crisis"};
                                        fusionName = doubleFusionCards[rand() % doubleFusionCards.size()];
                                    } else {
                                        // Normal fusion
                                        std::vector<std::string> fusionCards = {"Divine Arrow", "Void Storm", "Beast Rampage", "Cosmic Shield", "Blood Frenzy"};
                                        fusionName = fusionCards[rand() % fusionCards.size()];
                                    }
                                    
                                    // Add fusion card to draw pile (so you can use it!)
                                    deck.addCardToDrawPile(CardFactory::create(fusionName));
                                    
                                    // Remove the selected cards (highest index first!)
                                    for (size_t idx : forgeSelectedIndices) {
                                        if (idx < allCards.size()) {
                                            allCards.erase(allCards.begin() + idx);
                                        }
                                    }
                                    
                                    forgeSelectedIndices.clear();
                                }
                            }
                            break;
                        }
                    }
                    
                    if (continueButton.contains(mousePos)) {
                        forgeSelectedIndices.clear();
                        gameState = GameState::Combat;
                    }
                }
            }
        }

        // enemy turn
        if (gameState == GameState::Combat &&
            turnState == TurnState::EnemyTurn)
        {
            

            bool isStunned = false;
            for (auto& s : enemy.getState().statuses) {
                if (s->getType() == StatusType::Stun) {
                    isStunned = true;
                    break;
                }
            }

            if (enemy.getState().hp.current > 0 && !isStunned) {
                enemy.playTurn(playerState);
            } else {
                if (isStunned) {
                    float currentW = static_cast<float>(window.getSize().x);
                    float currentH = static_cast<float>(window.getSize().y);

                    sf::Vector2f enemyPos(currentW / 2.f, currentH / 4.f);
                    ui.spawnFCT(enemyPos, "Stunned!", sf::Color::Yellow, font);

                    // still pick a new intent while stunned so the next player turn shows it
                    enemy.rollIntent();
                }
            }

            enemy.getState().updateStatuses();

            // end thah turn
            enemy.getState().endTurn(enemy.getState());

            turnState = TurnState::PlayerTurn;
            playerState.updateStatuses();

            deck.discardHand();
            deck.drawCard(4);
            playerState.mana.current = playerState.mana.max;
            playerState.corruption.current = 0;
            if (playerState.isTransformed) {
                playerState.transformTime--;
                if (playerState.transformTime <= 0) {
                    playerState.isTransformed = false;
                }
            }
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

            lootOptions.clear();

            std::vector<std::string> lootNames = CharacterFactory::getRandomLootOptions(3);
            for(auto& name : lootNames) {
                lootOptions.push_back(CardFactory::create(name));
            }

            gameState = GameState::Looting;
        }


        // render
        window.clear(sf::Color(40, 40, 40));

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
        else if (gameState == GameState::ClassSelect) {
            float buttonWidth = 220.f;
            float buttonHeight = 70.f;
            float centerX = winW / 2.f - buttonWidth / 2.f;
            float startY = winH / 2.f - 120.f;
            float spacing = 100.f;

            std::vector<std::pair<std::string, Class>> classes = {
                {"Warrior", Class::Warrior},
                {"Cleric", Class::Cleric},
                {"Mage", Class::Mage},
                {"Ranger", Class::Ranger}
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
        else if (gameState == GameState::CharacterSelect) {
            float buttonWidth = 220.f;
            float buttonHeight = 70.f;
            float centerX = winW / 2.f - buttonWidth / 2.f;
            float startY = winH / 2.f - 120.f;
            float spacing = 100.f;

            std::vector<std::pair<std::string, CharacterName>> characters = {
                {"Hiroshi", CharacterName::Hiroshi},
                {"Phlox", CharacterName::Phlox},
                {"Vortex", CharacterName::Vortex},
                {"Mathews Lift", CharacterName::MathewsLift}
            };

            for (int i = 0; i < characters.size(); ++i) {

                sf::RectangleShape button({buttonWidth, buttonHeight});
                button.setPosition({centerX, startY + i * spacing});
                button.setFillColor(sf::Color(60,60,80));
                button.setOutlineColor(sf::Color::Black);
                button.setOutlineThickness(3.f);
                window.draw(button);

                sf::Text text(font, characters[i].first, 28);
                text.setPosition({centerX + 40.f, startY + i * spacing + 18.f});
                text.setFillColor(sf::Color::White);
                window.draw(text);
            }
        }
        // render combat screen
        else if (gameState == GameState::Combat) {
            corruptionParticles.update(dt);

            enemy.render(window, winW, winH);
            corruptionParticles.draw(window);
            deck.render(window, winW, winH, font, playerState, corruptionParticles);

            if (player.has_value()) {
                ui.render(window, *player, playerState, enemy.getState(), winW, winH, font,
                          enemy.getIntentDescription());
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            Card* hoveredCard = nullptr;
            for (Card& card : deck.getHand()) {
                if (card.getBounds().contains(mousePos)) {
                    hoveredCard = &card;
                }
            }

            ui.updateAndDrawFCT(window, dt);

            if (hoveredCard) {
                ui.drawTooltip(window, font, *hoveredCard, mousePos.x, mousePos.y);
            }
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

            // render the 3 cards in the middle of the screen
            int cardW = winW / 10;
            int cardH = winH / 6;
            float gap = cardW + 20.f;
            float totalWidth = (lootOptions.size() * gap) - 20.f;

            float startX = (winW - totalWidth) / 2.f;
            float cardY = winH * 0.7f;

            for (size_t i = 0; i < lootOptions.size(); ++i) {
                lootOptions[i].x = startX + (i * gap);
                lootOptions[i].y = cardY;
                lootOptions[i].w = cardW;
                lootOptions[i].h = cardH;

                lootOptions[i].draw(window, font, false, playerState);
            }

            // draw tooltip if hovered
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            Card* hoveredCard = nullptr;
            for (Card& card : lootOptions) {
                if (card.getBounds().contains(mousePos)) {
                    hoveredCard = &card;
                }
            }

            if (hoveredCard) {
                ui.drawTooltip(window, font, *hoveredCard, mousePos.x, mousePos.y);
            }
        }
        // render forge screen
        else if (gameState == GameState::Forge) {
            sf::Text title(font, "THE FORGE", (winW / 10));
            title.setPosition({winW / 2.f - 140.f, winH * 0.1f});
            title.setFillColor(sf::Color(255, 180, 50));
            window.draw(title);

            sf::Text subtitle(font, "Click 2 cards to fuse them into something powerful!", (winW / 30));
            subtitle.setPosition({winW / 2.f - 220.f, winH * 0.18f});
            subtitle.setFillColor(sf::Color::White);
            window.draw(subtitle);

            // redner card player collected
            auto& allCards = deck.getPermanentCollection();
            int cardW = winW / 10;
            int cardH = winH / 6;

            // display cards in grid
            int cols = 8;
            float startX = winW * 0.05f;
            float startY = winH * 0.25f;
            float gap = cardW + 15.f;

            for (size_t i = 0; i < allCards.size(); ++i) {
                int col = i % cols;
                int row = i / cols;

                allCards[i].x = startX + col * gap;
                allCards[i].y = startY + row * (cardH + 30.f);
                allCards[i].w = cardW;
                allCards[i].h = cardH;

                // check if card selected
                bool isSelected = std::find(forgeSelectedIndices.begin(), forgeSelectedIndices.end(), i) != forgeSelectedIndices.end();

                // draw highlight
                if (isSelected) {
                    sf::RectangleShape selectionHighlight(sf::Vector2f(cardW + 8.f, cardH + 8.f));
                    selectionHighlight.setPosition(sf::Vector2f(allCards[i].x - 4.f, allCards[i].y - 4.f));
                    selectionHighlight.setFillColor(sf::Color::Transparent);
                    selectionHighlight.setOutlineColor(sf::Color(255, 215, 0)); // Gold
                    selectionHighlight.setOutlineThickness(4.f);
                    window.draw(selectionHighlight);
                }

                allCards[i].draw(window, font, false, playerState);
            }

            // continue buttin
            sf::RectangleShape continueButton({200.f, 60.f});
            continueButton.setPosition({winW / 2.f - 100.f, winH * 0.85f});
            continueButton.setFillColor(sf::Color(70,70,70));
            continueButton.setOutlineColor(sf::Color::Black);
            continueButton.setOutlineThickness(3.f);
            window.draw(continueButton);

            sf::Text cont(font, "Continue", 30);
            cont.setPosition({winW / 2.f - 70.f, winH * 0.85f + 15.f});
            cont.setFillColor(sf::Color::White);
            window.draw(cont);
        }
        window.display();
    }

    return 0;
}
