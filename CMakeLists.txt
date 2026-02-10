cmake_minimum_required(VERSION 3.10)
project(card_engine)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2 via vcpkg
find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

# Paths for SDL2_ttf
set(SDL2_TTF_INCLUDE_DIR "C:/Users/Sem/vcpkg/installed/x64-mingw-static/include/SDL2")
set(SDL2_TTF_LIB "C:/Users/Sem/vcpkg/installed/x64-mingw-static/lib/libSDL2_ttf.a")

# Add executable
add_executable(card_engine
    src/main.cpp
    src/Card.cpp
    src/Deck.cpp
    src/UIRenderer.cpp
)

# Include your own headers + SDL2 headers
target_include_directories(card_engine PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(card_engine PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIB} ${FREETYPE_LIBRARIES})

