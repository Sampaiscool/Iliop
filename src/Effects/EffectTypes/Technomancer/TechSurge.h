#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"
#include <random>

class TechSurge : public Effect {
public:
    TechSurge(int value) : amount(value) {}
    TechSurge() = default;
    
    void apply(CombatState& self, CombatState& target, int value) override {
        if (self.isTransformed) {
            std::vector<std::string> upgrades = {"Overclocked", "Bleeding", "Shocking", "Vampiric", "Armored", "True Void", "Damage Up", "Defence Up"};
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, upgrades.size() - 1);
            int idx = dis(gen);
            
            self.applyStatus(std::make_unique<MachineUpgradeStatus>(2, 1, upgrades[idx]));
        }
    }
private:
    int amount = 1;
};
