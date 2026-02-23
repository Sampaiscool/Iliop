cmake_minimum_required(VERSION 3.15)
project(CardEngineSFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# linux
set(SFML_STATIC_LIBRARIES ON)

# windows
#set(SFML_DIR "C:/Libraries/SFML3/lib/cmake/SFML")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(card_engine
    src/main.cpp
    src/Other/CombatState.cpp
    src/Deck/Deck.cpp
    src/Card/Card.cpp
    src/Enemy/Enemy.cpp
    src/UI/UIRenderer.cpp
    src/Managers/CardResolver.cpp
    src/Managers/EnemyFactory.cpp
)

target_link_libraries(card_engine
    SFML::Graphics
    SFML::Window
    SFML::System
)
