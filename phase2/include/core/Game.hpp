/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <random>
#include "core/ResourceManager.hpp"
#include "core/Difficulty.hpp"
#include "core/SettingsManager.hpp"
#include "entities/Player.hpp"
#include "managers/PlatformManager.hpp"
#include "managers/UiManager.hpp"
#include "managers/ScoreManager.hpp"
#include "managers/BulletManager.hpp"
#include "managers/MonsterManager.hpp"
#include "managers/HoleManager.hpp"
#include "managers/SoundManager.hpp"
#include "managers/ComboManager.hpp"

enum class GameState {
    Menu,
    Gameplay,
    GameOver,
    Settings // Phase 2 (spec §5)
};

class Game {
public:
    Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void run();

private:
    void loadResources();

    void processEvents();
    void update(float deltaTime);
    void render();

    void updateGameplay(float deltaTime);
    void handleMouseClick(const sf::Vector2f& worldPos);

    void startNewGame();
    void triggerGameOver(bool playLoseSound = true);

    // ── Phase 2 helpers ──────────────────────────────────────────────────
    void updateShooting(float deltaTime);
    void updateSuckSequence(float deltaTime);
    float currentFireRate() const;

    sf::RenderWindow window;
    sf::View gameView;
    float viewTopY;

    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;

    GameState currentState;
    GameState stateBeforeSettings; // where "Back" should return to

    Player player;
    std::mt19937 randomEngine;
    PlatformManager platformManager;
    UiManager uiManager;
    ScoreManager scoreManager;

    float previousPlayerBottom;
    sf::Clock frameClock;

    // ── Phase 2 members ──────────────────────────────────────────────────
    SettingsManager settingsManager;
    SoundManager soundManager;
    BulletManager bulletManager;
    MonsterManager monsterManager;
    HoleManager holeManager;
    ComboManager comboManager; 

    bool firePressed;
    float timeSinceLastShot;
    float runElapsedTime; 


    bool isBeingSucked;
    sf::Vector2f suckStartPosition;
    sf::Vector2f suckTargetPosition;
    float suckTimer;

    bool draggingVolume; 
};

#endif
