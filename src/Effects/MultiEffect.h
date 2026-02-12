#pragma once
#include "Effect.h"

class MultiEffect : public Effect {
    std::vector<std::unique_ptr<Effect>> effects;

public:
    void add(std::unique_ptr<Effect> e) {
        effects.push_back(std::move(e));
    }

    void apply(CombatState& self, CombatState& target, bool isCorrupted) override {
        for (auto& e : effects)
            e->apply(self, target, isCorrupted);
    }
};
