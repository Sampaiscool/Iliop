cmake_minimum_required(VERSION 3.10)
project(card_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(card_engine
    src/main.cpp
    src/Card.cpp
    src/Deck.cpp
)

target_include_directories(card_engine PRIVATE ${SDL2_INCLUDE_DIRS} .)
target_link_libraries(card_engine PRIVATE SDL2::SDL2)

