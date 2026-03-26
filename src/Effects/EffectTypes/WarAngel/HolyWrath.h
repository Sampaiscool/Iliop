#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"

class HolyWrath : public Effect {
public:
    HolyWrath(int value) : amount(value) {}
    HolyWrath() = default;
    
    void apply(CombatState& self, CombatState& target, int value) override {
        if (self.isTransformed) {
            // When transformed, gain blessed + heal based on holy spirit
            int holySpirit = 0;
            for (auto& s : self.statuses) {
                if (s && s->name == "Holy Spirit") {
                    holySpirit = s->intensity;
                    break;
                }
            }
            self.applyStatus(std::make_unique<BlessedStatus>(2, 1));
            if (holySpirit > 0) {
                self.heal(holySpirit);
            }
        }
    }
private:
    int amount = 1;
};
