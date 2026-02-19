#pragma once
#include "Effect.h"
#include <vector>
#include <memory>

class MultiEffect : public Effect {
    std::vector<std::unique_ptr<Effect>> effects;

public:
    void add(std::unique_ptr<Effect> e) {
        effects.push_back(std::move(e));
    }

    void apply(CombatState& self, CombatState& target, int value) override {
        for (auto& e : effects) {
            if (e) e->apply(self, target, value);
        }
    }
};
