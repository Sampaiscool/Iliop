cmake_minimum_required(VERSION 3.10)
project(card_engine)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2 and TTF
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Add your executable
add_executable(card_engine
    src/main.cpp
    src/Card.cpp
    src/Deck.cpp
    src/UIRenderer.cpp
)

# Include your own headers
target_include_directories(card_engine PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Include SDL2 and SDL2_ttf headers
target_include_directories(card_engine PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Link SDL2 and SDL2_ttf
target_link_libraries(card_engine PRIVATE ${SDL2_LIBRARIES} SDL2_ttf)

