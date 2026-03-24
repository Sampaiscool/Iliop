#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../Effects/Effect.h"
#include "../Other/CombatState.h"
#include "../Other/AllStatuses.h"

enum class CardType { 
    Damage,
    Heal,
    Shield,

    // special
    Fusion,
    DoubleFusion,

    // ranger
    PrimalArrow,
    Aim,
    ArrowVolley,
    Jump,
    Metamorphosis,
    PredatorsMaul,

    // mage
    UnstableVolley,
    EldritchBlast,
    VoidGrasp,
    AstralShift,
    VoidProwess,
    VoidEcho,

    // warrior
    AegisStrike,
    IronWill,
    ShatterSpleen,
    ShieldBash,
    WarriorPact,
    Fortify,

    // cleric
    BlindingLight,
    Condemn,
    Purge,
    Ritual,
    Redemption,

    // necromancer
    GraveCall,
    BoneSplinter,
    RottingBite,
    SoulHarvest,
    DeathMark,
    ArmySurge,
    CryptShield,
    RaiseDead,
    BoneLegion,
    CorpseExplosion,

    // alchemist
    PotionBrew,
    Toss,
    Reaction,
    CatalystBomb,

    // metals
    LeadCard,
    GoldCard,
    CopperCard,
    IronCard,
    MercuryCard,
    SilverCard,
    TinCard,

    // technomancer
    MachinePower,
    Overclock,
    BleedInject,
    ShockInject,
    VampiricInject,
    ArmorInject,
    VoidInject,
    PowerInject,
    DefensiveMode,
    SystemCrash,

    // war angel
    Betray,
    BlessedSword,
    EthernalFlight,
    GoldenLance,
    HolyArrows,
    Savior,
    SpiralSlash,
    SpiritSlash,
    DivinePunishment,

    // new class cards
    HuntersMark,
    ArcaneSurge,
    BloodRampage,
    DivineAegis,
    Plague,
    AcidSplash,
    Overdrive,
    CelestialWrath
};

enum class CardTheme {
    Red, Blue, Green, Purple, Gold, Gray, Brown, Teal
};

struct Card {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    std::string name;
    std::string description;
    CardType type;
    CardTheme theme;
    std::unique_ptr<Effect> effect;
    int value = 0;
    int corruptedValue = 0;
    int cost;
    bool isTemporary = false;

    // Orb augmentations
    int costReduction = 0;
    int bonusValue = 0;
    int drawOnUse = 0;
    int replayCount = 0;
    int freeOnceCount = 0;
    
    // Status on play
    int applyDamageUp = 0;
    int applyDefenceUp = 0;
    int applyRegen = 0;
    int applyBleed = 0;
    
    // Other
    int healOnUse = 0;
    int shieldOnUse = 0;

    Card() = default;

    // forbid copy
    Card(const Card&) = delete;
    Card& operator=(const Card&) = delete;

    // allow move
    Card(Card&&) noexcept = default;
    Card& operator=(Card&&) noexcept = default;

    bool contains(int mx, int my) const {
        return mx >= x && mx <= x + w &&
               my >= y && my <= y + h;
    }

    void play(CombatState& self, CombatState& target, bool isCorrupted) const {
        if (effect) {
            int totalPower = value + bonusValue;
            if (isCorrupted) {
                totalPower += corruptedValue;
            }
            effect->apply(self, target, totalPower);
            
            if (drawOnUse > 0) {
                self.draw(drawOnUse);
            }
            
            // apply statuses (stackable - apply multiple times based on intensity)
            for (int i = 0; i < applyDamageUp; i++) {
                self.applyStatus(std::make_unique<DamageUpStatus>(3, 2));
            }
            for (int i = 0; i < applyDefenceUp; i++) {
                self.applyStatus(std::make_unique<DefenceUpStatus>(3, 3));
            }
            for (int i = 0; i < applyRegen; i++) {
                self.applyStatus(std::make_unique<RegenerationStatus>(3, 3));
            }
            for (int i = 0; i < applyBleed; i++) {
                target.applyStatus(std::make_unique<BleedStatus>(3, 2));
            }
            
            // heal and shield
            if (healOnUse > 0) self.heal(healOnUse);
            if (shieldOnUse > 0) self.addShield(shieldOnUse);
        }
    }

    void draw(sf::RenderWindow& window, const sf::Font& font, bool isCorrupted, const CombatState& playerState) const;

    int getActiveValue(bool isCorrupted) const {
        // if you are corrupted add the bonus, else not
        return isCorrupted ? (value + corruptedValue) : value;
    }

    sf::FloatRect getBounds() const {
        return sf::FloatRect({(float)x, (float)y}, {(float)w, (float)h});
    }
};
