#pragma once

enum class GameState {
    StartScreen,
    ClassSelect,
    CharacterSelect,
    Combat,
    Victory,
    Looting,
    GameOver
};

enum class TurnState {
    PlayerTurn,
    EnemyTurn
};
