cmake_minimum_required(VERSION 3.10)
project(card_engine)

set(CMAKE_CXX_STANDARD 17)

# SDL2 core
find_package(SDL2 REQUIRED)

# SDL2_ttf (this is the important fix)
find_package(SDL2_ttf REQUIRED)

# Add executable
add_executable(card_engine
    src/main.cpp
    src/Card/Card.cpp
    src/Deck/Deck.cpp
    src/UI/UIRenderer.cpp
    src/Enemy/Enemy.cpp
    src/Managers/CardResolver.cpp
)

# Include directories
target_include_directories(card_engine PRIVATE
    ${SDL2_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(card_engine PRIVATE
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
)

