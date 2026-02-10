cmake_minimum_required(VERSION 3.10)
project(card_engine_sfml)

set(CMAKE_CXX_STANDARD 17)

# SFML 3
find_package(SFML 3 REQUIRED COMPONENTS graphics window system)

add_executable(card_engine_sfml
    src/main.cpp
    src/Deck/Deck.cpp
    src/Card/Card.cpp
    src/Enemy/Enemy.cpp
    src/UI/UIRenderer.cpp
    src/Managers/CardResolver.cpp
)

target_include_directories(card_engine_sfml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link SFML correctly
target_link_libraries(card_engine_sfml PRIVATE SFML::graphics SFML::window SFML::system)

