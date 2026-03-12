#pragma once
#include "../../Effect.h"
#include "../../../Other/CombatState.h"
#include "../../../Other/AllStatuses.h"
#include <algorithm>
#include <vector>

class Reaction : public Effect {
private:
    // reaction methods
    void applyBronze(CombatState& self, CombatState& target) {
        // Copper + Iron
        self.addShield(8);
        self.applyStatus(std::make_unique<DefenceUpStatus>(3, 3));
    }

    void applyElectrum(CombatState& self, CombatState& target) {
        // Gold + Silver
        self.heal(30);
    }

    void applyPewter(CombatState& self, CombatState& target) {
        // Lead + Tin
        target.takeDamage(self.getModifiedDamage(12));
        self.takeDamage(3);
    }

    void applyBrass(CombatState& self, CombatState& target) {
        // Copper + Tin
        self.applyStatus(std::make_unique<DamageUpStatus>(3, 3));
        target.takeDamage(self.getModifiedDamage(6));
    }

    void applySteel(CombatState& self, CombatState& target) {
        // Iron + Tin
        self.addShield(10);
        self.applyStatus(std::make_unique<DefenceUpStatus>(2, 2));
    }

    void applyQuicksilver(CombatState& self, CombatState& target) {
        // Mercury + Silver
        for (int i = 0; i < 3; i++) target.takeDamage(self.getModifiedDamage(4));
    }

    void applyVolatile(CombatState& self, CombatState& target) {
        // Iron + Mercury
        target.takeDamage(self.getModifiedDamage(15));
        self.takeDamage(5);
    }

    void applyMidas(CombatState& self, CombatState& target) {
        // Gold + Lead
        self.applyStatus(std::make_unique<DamageUpStatus>(2, 5));
        target.takeDamage(self.getModifiedDamage(8));
    }

    void applyAurichalceum(CombatState& self, CombatState& target) {
        // Copper + Gold
        target.takeDamage(self.getModifiedDamage(7));
        self.heal(7);
    }
    
    void applyVerdigris(CombatState& self, CombatState& target) {
        // Copper + Mercury
        target.applyStatus(std::make_unique<BleedStatus>(3, 3));
    }

    void applyAmalgam(CombatState& self, CombatState& target) {
        // Lead + Mercury
        target.takeDamage(self.getModifiedDamage(4));
        self.heal(3);
    }

    void applySolder(CombatState& self, CombatState& target) {
        // Iron + Lead
        target.applyStatus(std::make_unique<StunStatus>(1, 1));
    }

    void applyWhiteMetal(CombatState& self, CombatState& target) {
        // Copper + Lead
        target.applyStatus(std::make_unique<StunStatus>(1, 1));
        target.applyStatus(std::make_unique<BleedStatus>(2, 2));
    }

    void applyLiquidHeavy(CombatState& self, CombatState& target) {
        // Lead + Mercury
        target.applyStatus(std::make_unique<StunStatus>(1, 1));
        target.takeDamage(self.getModifiedDamage(8));
    }

    void applyHeavyShiny(CombatState& self, CombatState& target) {
        // Lead + Silver
        self.addShield(6);
        self.heal(6);
    }

    void applyElectrumSteel(CombatState& self, CombatState& target) {
        // Gold + Iron
        for (int i = 0; i < 2; i++) target.takeDamage(self.getModifiedDamage(5));
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, 3));
    }

    void applyShimmering(CombatState& self, CombatState& target) {
        // Gold + Mercury
        self.applyStatus(std::make_unique<TrueVoidStatus>(2, 3));
        self.heal(5);
    }

    void applyBright(CombatState& self, CombatState& target) {
        // Gold + Tin
        self.applyStatus(std::make_unique<DamageUpStatus>(2, 4));
        target.takeDamage(self.getModifiedDamage(5));
    }

    void applyConducting(CombatState& self, CombatState& target) {
        // Copper + Silver
        self.mana.current = std::min(self.mana.current + 1, self.mana.max);
        target.takeDamage(self.getModifiedDamage(8));
    }

    void applyShinyHard(CombatState& self, CombatState& target) {
        // Iron + Silver
        self.addShield(7);
        self.applyStatus(std::make_unique<DefenceUpStatus>(2, 2));
    }

    void applyFluid(CombatState& self, CombatState& target) {
        // Mercury + Tin
        target.takeDamage(self.getModifiedDamage(6));
        target.takeDamage(self.getModifiedDamage(6));
        self.takeDamage(2);
    }

    void applyBrittle(CombatState& self, CombatState& target) {
        // Silver + Tin
        target.takeDamage(self.getModifiedDamage(9));
        target.applyStatus(std::make_unique<DefenceDownStatus>(2, 2));
    }

    void applyWeak(CombatState& self, CombatState& target) {
        // Default
        target.takeDamage(self.getModifiedDamage(2));
        self.takeDamage(1);
    }

public:
    void apply(CombatState& self, CombatState& target, int value) override {
        std::vector<StatusType> metals;
        for (const auto& s : self.statuses) {
            if (!s) continue;

            StatusType type = s->getType();
            if (type == StatusType::Lead    || type == StatusType::Gold    ||
                type == StatusType::Copper  || type == StatusType::Iron    ||
                type == StatusType::Mercury || type == StatusType::Silver  ||
                type == StatusType::Tin) {
                metals.push_back(type);
            }
        }

        self.removeRandomMetalStatus();
        self.removeRandomMetalStatus();

        if (metals.size() < 2) {
            applyWeak(self, target);
            return;
        }

        std::sort(metals.begin(), metals.end());
        StatusType m1 = metals[0], m2 = metals[1];

        // check combinations
        if      (m1 == StatusType::Copper  && m2 == StatusType::Iron)    applyBronze(self, target);       // Copper + Iron
        else if (m1 == StatusType::Gold    && m2 == StatusType::Silver)  applyElectrum(self, target);     // Gold + Silver
        else if (m1 == StatusType::Lead    && m2 == StatusType::Tin)     applyPewter(self, target);       // Lead + Tin
        else if (m1 == StatusType::Copper  && m2 == StatusType::Tin)     applyBrass(self, target);        // Copper + Tin
        else if (m1 == StatusType::Iron    && m2 == StatusType::Tin)     applySteel(self, target);        // Iron + Tin
        else if (m1 == StatusType::Mercury && m2 == StatusType::Silver)  applyQuicksilver(self, target);  // Mercury + Silver
        else if (m1 == StatusType::Iron    && m2 == StatusType::Mercury) applyVolatile(self, target);     // Iron + Mercury
        else if (m1 == StatusType::Gold    && m2 == StatusType::Lead)    applyMidas(self, target);        // Gold + Lead
        else if (m1 == StatusType::Copper  && m2 == StatusType::Gold)    applyAurichalceum(self, target); // Copper + Gold
        else if (m1 == StatusType::Copper  && m2 == StatusType::Mercury) applyVerdigris(self, target);    // Copper + Mercury
        else if (m1 == StatusType::Lead    && m2 == StatusType::Mercury) applyAmalgam(self, target);      // Lead + Mercury
        else if (m1 == StatusType::Iron    && m2 == StatusType::Lead)    applySolder(self, target);       // Iron + Lead
        else if (m1 == StatusType::Copper  && m2 == StatusType::Lead)    applyWhiteMetal(self, target);   // Copper + Lead
        else if (m1 == StatusType::Lead    && m2 == StatusType::Mercury) applyLiquidHeavy(self, target);  // Lead + Mercury
        else if (m1 == StatusType::Lead    && m2 == StatusType::Silver)  applyHeavyShiny(self, target);   // Lead + Silver
        else if (m1 == StatusType::Gold    && m2 == StatusType::Iron)    applyElectrumSteel(self, target);// Gold + Iron
        else if (m1 == StatusType::Gold    && m2 == StatusType::Mercury) applyShimmering(self, target);   // Gold + Mercury
        else if (m1 == StatusType::Gold    && m2 == StatusType::Tin)     applyBright(self, target);       // Gold + Tin
        else if (m1 == StatusType::Copper  && m2 == StatusType::Silver)  applyConducting(self, target);   // Copper + Silver
        else if (m1 == StatusType::Iron    && m2 == StatusType::Silver)  applyShinyHard(self, target);    // Iron + Silver
        else if (m1 == StatusType::Mercury && m2 == StatusType::Tin)     applyFluid(self, target);        // Mercury + Tin
        else if (m1 == StatusType::Silver  && m2 == StatusType::Tin)     applyBrittle(self, target);      // Silver + Tin
        else applyWeak(self, target);
    }
};
