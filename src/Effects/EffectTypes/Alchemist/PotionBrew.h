#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"
#include <vector>
#include <random>

class PotionBrew : public Effect {
public:
    void apply(CombatState& self, CombatState& target, int value) override {
        std::vector<std::string> metals = {"Lead", "Gold", "Copper", "Iron", "Mercury", "Silver", "Tin"};

        // init the random loop 
        // magic
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, static_cast<int>(metals.size()) - 1);

        int firstIdx = dis(gen);
        int secondIdx;

        // just keep rollin rollin rollin
        do {
            secondIdx = dis(gen);
        } while (secondIdx == firstIdx);

        self.addCardToHand(metals[firstIdx]);
        self.addCardToHand(metals[secondIdx]);
    }
};
