#pragma once

enum class GameState {
    StartScreen,
    ClassSelect,
    CharacterSelect,
    Combat,
    Victory,
    Looting,
    Forge,
    Settings,
    GameOver
};

enum class TurnState {
    PlayerTurn,
    EnemyTurn
};
