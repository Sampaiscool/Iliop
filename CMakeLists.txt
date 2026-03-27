cmake_minimum_required(VERSION 3.15)
project(CardEngineSFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# force static build
set(SFML_STATIC_LIBRARIES ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/SFML)

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

# links the targets directly
target_link_libraries(card_engine
    sfml-graphics
    sfml-window
    sfml-system
)

# copy the assets map to build directory
add_custom_command(TARGET card_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:card_engine>/assets"
)

if(WIN32)
    # make sure the .exe doesnt open a command prompt
    set_target_properties(card_engine PROPERTIES WIN32_EXECUTABLE TRUE)

    # force static linking with C++ runtime 
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
