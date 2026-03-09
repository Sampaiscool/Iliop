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

    // screen numbers
    int classScreen = 0;
    int characterScreen = 0;

    // settings
    int targetFPS = 30;
    GameState previousState = GameState::Combat;

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

            // esc for settings
            if (auto key = event.getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) {
                    if (gameState == GameState::Combat ||
                        gameState == GameState::Looting ||
                        gameState == GameState::Forge ||
                        gameState == GameState::Victory) {
                        previousState = gameState;
                        gameState = GameState::Settings;
                    } else if (gameState == GameState::Settings) {
                        // go back
                        gameState = previousState;
                    }
                }
            }

            // mouse wheel scroll for statuses
            if (auto wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
                if (gameState == GameState::Combat) {
                    if (wheel->delta > 0) {
                        ui.scrollPlayerStatuses(-1);
                        ui.scrollEnemyStatuses(-1);
                    } else {
                        ui.scrollPlayerStatuses(1);
                        ui.scrollEnemyStatuses(1);
                    }
                }
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

                    // settings button
                    sf::FloatRect settingsButton(
                        sf::Vector2f(window.getSize().x - 120.f, 20.f),
                        sf::Vector2f(100.f, 40.f));

                    if (settingsButton.contains(mousePos)) {
                        previousState = GameState::StartScreen;
                        gameState = GameState::Settings;
                    }
                }
                else if (gameState == GameState::ClassSelect) {
                    int winW = window.getSize().x;
                    int winH = window.getSize().y;

                    float buttonWidth = 220.f;
                    float buttonHeight = 70.f;
                    float centerX = winW / 2.f - buttonWidth / 2.f;
                    float startY = winH / 2.f - 200.f;
                    float spacing = 100.f;

                    // add < and > buttons for screen switching
                    // < button 
                    sf::FloatRect prevButton(
                        sf::Vector2f(winW * 0.1f, winH / 2.f - 30.f),
                        sf::Vector2f(60.f, 60.f));
                    if (prevButton.contains(mousePos) && classScreen > 0) {
                        classScreen--;
                    }

                    // > button
                    sf::FloatRect nextButton(
                        sf::Vector2f(winW * 0.85f, winH / 2.f - 30.f),
                        sf::Vector2f(60.f, 60.f));
                    if (nextButton.contains(mousePos) && classScreen < 1) {
                        classScreen++;
                    }

                    std::vector<std::pair<std::string, Class>> classList = {
                        {"Warrior", Class::Warrior},
                        {"Cleric", Class::Cleric},
                        {"Mage", Class::Mage},
                        {"Ranger", Class::Ranger},
                        {"Necromancer", Class::Necromancer},
                        {"Bard", Class::Bard},
                        {"Paladin", Class::Paladin},
                        {"Rogue", Class::Rogue}
                    };

                    for (int i = 0; i < 4; ++i) {
                        int actualIndex = classScreen * 4 + i;
                        if (actualIndex >= (int)classList.size()) break;

                        int displayIndex = i;

                        sf::FloatRect bounds(
                            sf::Vector2f(centerX, startY + displayIndex * spacing),
                            sf::Vector2f(buttonWidth, buttonHeight)
                        );

                        if (bounds.contains(mousePos)) {
                            selectedClass = classList[actualIndex].second;
                            characterScreen = 0;
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
                    float startY = winH / 2.f - 200.f;
                    float spacing = 100.f;

                    // < and > buttons for character screen switching
                    sf::FloatRect prevCharButton(
                        sf::Vector2f(winW * 0.1f, winH / 2.f - 30.f),
                        sf::Vector2f(60.f, 60.f));
                    if (prevCharButton.contains(mousePos) && characterScreen > 0) {
                        characterScreen--;
                    }

                    sf::FloatRect nextCharButton(
                        sf::Vector2f(winW * 0.85f, winH / 2.f - 30.f),
                        sf::Vector2f(60.f, 60.f));
                    if (nextCharButton.contains(mousePos) && characterScreen < 1) {
                        characterScreen++;
                    }

                    std::vector<std::pair<std::string, CharacterName>> charactersList = {
                        {"Hiroshi", CharacterName::Hiroshi},
                        {"Phlox", CharacterName::Phlox},
                        {"Vortex", CharacterName::Vortex},
                        {"Mathews Lift", CharacterName::MathewsLift},
                        {"Djin", CharacterName::Djin},
                        {"doobie2", CharacterName::doobie2},
                        {"doobie3", CharacterName::doobie3},
                        {"doobie4", CharacterName::doobie4}
                    };

                    for (int i = 0; i < 4; ++i) {
                        int actualIndex = characterScreen * 4 + i;
                        if (actualIndex >= (int)charactersList.size()) break;

                        int displayIndex = i;

                        sf::FloatRect bounds(
                            sf::Vector2f(centerX, startY + displayIndex * spacing),
                            sf::Vector2f(buttonWidth, buttonHeight)
                        );

                        if (bounds.contains(mousePos)) {

                            Character character = CharacterFactory::create(selectedClass, charactersList[actualIndex].second);

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
                        enemy.rollIntent();

                        ui.resetHPTracking();
                        ui.resetStatusScrolls();

                        // go to forge after new floor
                        if (currentFloor > 1 && (currentFloor - 1) % 3 == 0) {
                            gameState = GameState::Forge;
                        } else {
                            gameState = GameState::Combat;
                        }
                    }
                }
                // forge
                else if (gameState == GameState::Forge) {
                    sf::FloatRect continueButton(
                        sf::Vector2f(window.getSize().x / 2.f - 100.f, window.getSize().y * 0.85f),
                        sf::Vector2f(200.f, 60.f));

                    // get all cards from permanent collection
                    auto& allCards = deck.getPermanentCollection();

                    for (size_t i = 0; i < allCards.size(); ++i) {
                        sf::FloatRect cardRect({(float)allCards[i].x, (float)allCards[i].y},
                                              {(float)allCards[i].w, (float)allCards[i].h});

                        if (cardRect.contains(mousePos)) {
                            // check if already selected
                            auto it = std::find(forgeSelectedIndices.begin(), forgeSelectedIndices.end(), i);
                            if (it != forgeSelectedIndices.end()) {
                                forgeSelectedIndices.erase(it);
                            } else if (forgeSelectedIndices.size() < 2) {
                                forgeSelectedIndices.push_back(i);

                                // if 2 selected, fuse them
                                if (forgeSelectedIndices.size() == 2) {
                                    // sort indices descending so we can remove from back first
                                    std::sort(forgeSelectedIndices.begin(), forgeSelectedIndices.end(), std::greater<size_t>());

                                    // check if both cards are already fusions for the double fusion :)
                                    std::string name1 = allCards[forgeSelectedIndices[0]].name;
                                    std::string name2 = allCards[forgeSelectedIndices[1]].name;

                                    bool isFusion1 = (name1 == "Divine Arrow" || name1 == "Void Storm" || 
                                                     name1 == "Beast Rampage" || name1 == "Cosmic Shield" || name1 == "Blood Frenzy");
                                    bool isFusion2 = (name2 == "Divine Arrow" || name2 == "Void Storm" || 
                                                     name2 == "Beast Rampage" || name2 == "Cosmic Shield" || name2 == "Blood Frenzy");

                                    std::string fusionName;
                                    if (isFusion1 && isFusion2) {
                                        // double
                                        std::vector<std::string> doubleFusionCards = {"Omega Annihilation", "Universal Singularity", "Primordial Chaos", "Existential Crisis"};
                                        fusionName = doubleFusionCards[rand() % doubleFusionCards.size()];
                                    } else {
                                        // normal
                                        std::vector<std::string> fusionCards = {"Divine Arrow", "Void Storm", "Beast Rampage", "Cosmic Shield", "Blood Frenzy"};
                                        fusionName = fusionCards[rand() % fusionCards.size()];
                                    }
                                    // add card to draw + permanent
                                    deck.addCardToDrawPile(CardFactory::create(fusionName));
                                    deck.addCardToPermanentCollection(CardFactory::create(fusionName));
                                    // remove the selected cards
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
                // setting menu click
                else if (gameState == GameState::Settings) {
                    // fps toggle button
                    sf::FloatRect fpsButton(
                        sf::Vector2f(winW / 2.f - 100.f, winH * 0.3f),
                        sf::Vector2f(200.f, 50.f));

                    if (fpsButton.contains(mousePos)) {
                        // toggle tha fps
                        if (targetFPS == 60) targetFPS = 30;
                        else if (targetFPS == 30) targetFPS = 144;
                        else targetFPS = 60;
                        window.setFramerateLimit(targetFPS);
                    }

                    // restart button
                    sf::FloatRect restartButton(
                        sf::Vector2f(winW / 2.f - 100.f, winH * 0.45f),
                        sf::Vector2f(200.f, 50.f));

                    if (restartButton.contains(mousePos)) {
                        // restarttt!
                        currentFloor = 1;
                        deck = Deck();
                        player.reset();
                        gameState = GameState::StartScreen;
                        forgeSelectedIndices.clear();
                    }

                    // quit button
                    sf::FloatRect quitButton(
                        sf::Vector2f(winW / 2.f - 100.f, winH * 0.6f),
                        sf::Vector2f(200.f, 50.f));

                    if (quitButton.contains(mousePos)) {
                        window.close();
                    }

                    // back button
                    sf::FloatRect backButton(
                        sf::Vector2f(winW / 2.f - 100.f, winH * 0.75f),
                        sf::Vector2f(200.f, 50.f));

                    if (backButton.contains(mousePos)) {
                        gameState = previousState;
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

            // settings button
            sf::RectangleShape settingsButton({100.f, 40.f});
            settingsButton.setPosition({winW - 120.f, 20.f});
            settingsButton.setFillColor(sf::Color(60, 60, 80));
            settingsButton.setOutlineColor(sf::Color::Black);
            settingsButton.setOutlineThickness(2.f);
            window.draw(settingsButton);

            sf::Text settingsText(font, "Settings", 18);
            settingsText.setPosition({winW - 110.f, 28.f});
            settingsText.setFillColor(sf::Color::White);
            window.draw(settingsText);
        }
        else if (gameState == GameState::ClassSelect) {
            float buttonWidth = 220.f;
            float buttonHeight = 70.f;
            float centerX = winW / 2.f - buttonWidth / 2.f;
            float startY = winH / 2.f - 200.f;
            float spacing = 100.f;

            std::vector<std::pair<std::string, Class>> classes = {
                {"Warrior", Class::Warrior},
                {"Cleric", Class::Cleric},
                {"Mage", Class::Mage},
                {"Ranger", Class::Ranger},
                {"Necromancer", Class::Necromancer},
                {"Bard", Class::Bard},
                {"Paladin", Class::Paladin},
                {"Rogue", Class::Rogue}
            };

            for (int i = 0; i < 4; ++i) {
                int actualIndex = classScreen * 4 + i;
                if (actualIndex >= (int)classes.size()) break;

                int displayIndex = i;

                sf::RectangleShape button({buttonWidth, buttonHeight});
                button.setPosition({centerX, startY + displayIndex * spacing});
                button.setFillColor(sf::Color(60,60,80));
                button.setOutlineColor(sf::Color::Black);
                button.setOutlineThickness(3.f);
                window.draw(button);

                sf::Text text(font, classes[actualIndex].first, 28);
                text.setPosition({centerX + 40.f, startY + displayIndex * spacing + 18.f});
                text.setFillColor(sf::Color::White);
                window.draw(text);
            }

            // draw < and > buttons
            sf::RectangleShape prevArrowBtn({60.f, 60.f});
            prevArrowBtn.setPosition({winW * 0.1f, winH / 2.f - 30.f});
            prevArrowBtn.setFillColor(sf::Color(60, 60, 80));
            prevArrowBtn.setOutlineColor(sf::Color::Black);
            prevArrowBtn.setOutlineThickness(2.f);
            if (classScreen > 0) window.draw(prevArrowBtn);

            sf::Text prevArrow(font, "<", 40);
            prevArrow.setPosition({winW * 0.1f + 15.f, winH / 2.f - 25.f});
            prevArrow.setFillColor(sf::Color::White);
            if (classScreen > 0) window.draw(prevArrow);

            sf::RectangleShape nextArrowBtn({60.f, 60.f});
            nextArrowBtn.setPosition({winW * 0.85f, winH / 2.f - 30.f});
            nextArrowBtn.setFillColor(sf::Color(60, 60, 80));
            nextArrowBtn.setOutlineColor(sf::Color::Black);
            nextArrowBtn.setOutlineThickness(2.f);
            if (classScreen < 1) window.draw(nextArrowBtn);

            sf::Text nextArrow(font, ">", 40);
            nextArrow.setPosition({winW * 0.85f + 20.f, winH / 2.f - 25.f});
            nextArrow.setFillColor(sf::Color::White);
            if (classScreen < 1) window.draw(nextArrow);

            // Screen indicator
            sf::Text screenIndicator(font, "Page " + std::to_string(classScreen + 1) + "/2", 20);
            screenIndicator.setPosition({winW / 2.f - 30.f, winH * 0.08f});
            screenIndicator.setFillColor(sf::Color::White);
            window.draw(screenIndicator);
        }
        else if (gameState == GameState::CharacterSelect) {
            float buttonWidth = 220.f;
            float buttonHeight = 70.f;
            float centerX = winW / 2.f - buttonWidth / 2.f;
            float startY = winH / 2.f - 200.f;
            float spacing = 100.f;

            std::vector<std::pair<std::string, CharacterName>> characters = {
                {"Hiroshi", CharacterName::Hiroshi},
                {"Phlox", CharacterName::Phlox},
                {"Vortex", CharacterName::Vortex},
                {"Mathews Lift", CharacterName::MathewsLift},
                {"Djin", CharacterName::Djin},
                {"doobie2", CharacterName::doobie2},
                {"doobie3", CharacterName::doobie3},
                {"doobie4", CharacterName::doobie4}
            };

            // draw < and > buttons
            sf::RectangleShape prevCharArrowBtn({60.f, 60.f});
            prevCharArrowBtn.setPosition({winW * 0.1f, winH / 2.f - 30.f});
            prevCharArrowBtn.setFillColor(sf::Color(60, 60, 80));
            prevCharArrowBtn.setOutlineColor(sf::Color::Black);
            prevCharArrowBtn.setOutlineThickness(2.f);
            if (characterScreen > 0) window.draw(prevCharArrowBtn);

            sf::Text prevCharArrow(font, "<", 40);
            prevCharArrow.setPosition({winW * 0.1f + 15.f, winH / 2.f - 25.f});
            prevCharArrow.setFillColor(sf::Color::White);
            if (characterScreen > 0) window.draw(prevCharArrow);

            sf::RectangleShape nextCharArrowBtn({60.f, 60.f});
            nextCharArrowBtn.setPosition({winW * 0.85f, winH / 2.f - 30.f});
            nextCharArrowBtn.setFillColor(sf::Color(60, 60, 80));
            nextCharArrowBtn.setOutlineColor(sf::Color::Black);
            nextCharArrowBtn.setOutlineThickness(2.f);
            if (characterScreen < 1) window.draw(nextCharArrowBtn);

            sf::Text nextCharArrow(font, ">", 40);
            nextCharArrow.setPosition({winW * 0.85f + 20.f, winH / 2.f - 25.f});
            nextCharArrow.setFillColor(sf::Color::White);
            if (characterScreen < 1) window.draw(nextCharArrow);

            // screen indicator
            sf::Text charScreenIndicator(font, "Page " + std::to_string(characterScreen + 1) + "/2", 20);
            charScreenIndicator.setPosition({winW / 2.f - 30.f, winH * 0.08f});
            charScreenIndicator.setFillColor(sf::Color::White);
            window.draw(charScreenIndicator);

            for (int i = 0; i < 4; ++i) {
                int actualIndex = characterScreen * 4 + i;
                if (actualIndex >= (int)characters.size()) break;

                int displayIndex = i;

                sf::RectangleShape button({buttonWidth, buttonHeight});
                button.setPosition({centerX, startY + displayIndex * spacing});
                button.setFillColor(sf::Color(60,60,80));
                button.setOutlineColor(sf::Color::Black);
                button.setOutlineThickness(3.f);
                window.draw(button);

                sf::Text text(font, characters[actualIndex].first, 28);
                text.setPosition({centerX + 40.f, startY + displayIndex * spacing + 18.f});
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

            // render card player collected
            auto& allCards = deck.getPermanentCollection();
            int cardW = winW / 10;
            int cardH = winH / 6;

            std::vector<std::string> fusionNames = {"Divine Arrow", "Void Storm", "Beast Rampage", "Cosmic Shield", "Blood Frenzy"};

            // display cards in grid
            int cols = 8;
            float startX = winW * 0.05f;
            float startY = winH * 0.25f;
            float gap = cardW + 15.f;

            int displayIndex = 0;
            for (size_t i = 0; i < allCards.size(); ++i) {
                // skip thah double fusions
                bool isDoubleFusion = (allCards[i].name == "Omega Annihilation" || 
                                       allCards[i].name == "Universal Singularity" ||
                                       allCards[i].name == "Primordial Chaos" ||
                                       allCards[i].name == "Existential Crisis");
                if (isDoubleFusion) continue;

                int col = displayIndex % cols;
                int row = displayIndex / cols;

                allCards[i].x = startX + col * gap;
                allCards[i].y = startY + row * (cardH + 30.f);
                allCards[i].w = cardW;
                allCards[i].h = cardH;
                displayIndex++;

                // check if card selected
                bool isSelected = std::find(forgeSelectedIndices.begin(), forgeSelectedIndices.end(), i) != forgeSelectedIndices.end();

                // draw highlight
                if (isSelected) {
                    sf::RectangleShape selectionHighlight(sf::Vector2f(cardW + 8.f, cardH + 8.f));
                    selectionHighlight.setPosition(sf::Vector2f(allCards[i].x - 4.f, allCards[i].y - 4.f));
                    selectionHighlight.setFillColor(sf::Color::Transparent);
                    selectionHighlight.setOutlineColor(sf::Color(255, 215, 0));
                    selectionHighlight.setOutlineThickness(4.f);
                    window.draw(selectionHighlight);
                }

                allCards[i].draw(window, font, false, playerState);
            }

            // draw tooltip if hovered
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            Card* hoveredCard = nullptr;
            for (Card& card : allCards) {
                if (card.getBounds().contains(mousePos)) {
                    hoveredCard = &card;
                }
            }

            if (hoveredCard) {
                ui.drawTooltip(window, font, *hoveredCard, mousePos.x, mousePos.y);
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
        // render settings screen
        else if (gameState == GameState::Settings) {
            sf::RectangleShape bg(sf::Vector2f(winW, winH));
            bg.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(bg);
            
            // title
            sf::Text title(font, "SETTINGS", 50);
            title.setPosition({winW / 2.f - 120.f, winH * 0.15f});
            title.setFillColor(sf::Color::White);
            window.draw(title);
            
            // fps button
            sf::RectangleShape fpsButton({200.f, 50.f});
            fpsButton.setPosition({winW / 2.f - 100.f, winH * 0.3f});
            fpsButton.setFillColor(sf::Color(60, 60, 80));
            fpsButton.setOutlineColor(sf::Color::Black);
            fpsButton.setOutlineThickness(3.f);
            window.draw(fpsButton);
            
            sf::Text fpsText(font, "FPS: " + std::to_string(targetFPS), 24);
            fpsText.setPosition({winW / 2.f - 60.f, winH * 0.3f + 12.f});
            fpsText.setFillColor(sf::Color::White);
            window.draw(fpsText);
            
            // restart button
            sf::RectangleShape restartButton({200.f, 50.f});
            restartButton.setPosition({winW / 2.f - 100.f, winH * 0.45f});
            restartButton.setFillColor(sf::Color(80, 60, 60));
            restartButton.setOutlineColor(sf::Color::Black);
            restartButton.setOutlineThickness(3.f);
            window.draw(restartButton);
            
            sf::Text restartText(font, "Restart", 24);
            restartText.setPosition({winW / 2.f - 55.f, winH * 0.45f + 12.f});
            restartText.setFillColor(sf::Color::White);
            window.draw(restartText);
            
            // quit button
            sf::RectangleShape quitButton({200.f, 50.f});
            quitButton.setPosition({winW / 2.f - 100.f, winH * 0.6f});
            quitButton.setFillColor(sf::Color(60, 60, 60));
            quitButton.setOutlineColor(sf::Color::Black);
            quitButton.setOutlineThickness(3.f);
            window.draw(quitButton);
            
            sf::Text quitText(font, "Quit", 24);
            quitText.setPosition({winW / 2.f - 35.f, winH * 0.6f + 12.f});
            quitText.setFillColor(sf::Color::White);
            window.draw(quitText);
            
            // back button
            sf::RectangleShape backButton({200.f, 50.f});
            backButton.setPosition({winW / 2.f - 100.f, winH * 0.75f});
            backButton.setFillColor(sf::Color(60, 80, 60));
            backButton.setOutlineColor(sf::Color::Black);
            backButton.setOutlineThickness(3.f);
            window.draw(backButton);
            
            sf::Text backText(font, "Back", 24);
            backText.setPosition({winW / 2.f - 45.f, winH * 0.75f + 12.f});
            backText.setFillColor(sf::Color::White);
            window.draw(backText);
            
            // esc hint
            sf::Text escHint(font, "Press ESC to go back", 18);
            escHint.setPosition({winW / 2.f - 90.f, winH * 0.9f});
            escHint.setFillColor(sf::Color(150, 150, 150));
            window.draw(escHint);
        }
        window.display();
    }

    return 0;
}
