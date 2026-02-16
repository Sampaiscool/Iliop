#pragma once
#include "Resource.h"
#include <memory>

class Effect;

struct CombatState {
    Resource hp;
    Resource shield;
    Resource mana;
    Resource corruption;
    int transformThreshold;
    bool isTransformed = false;
    int transformTime;
    int transformGain;

    std::unique_ptr<Effect> transformationProc;
};
