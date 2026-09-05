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
#include "entities/Player.hpp"
#include "managers/PlatformManager.hpp"
#include "managers/UiManager.hpp"
#include "managers/ScoreManager.hpp"

enum class GameState {
    Menu,
    Gameplay,
    GameOver
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
    void triggerGameOver();

    sf::RenderWindow window;
    sf::View gameView;
    float viewTopY;

    ResourceManager<sf::Texture> textures;
    ResourceManager<sf::Font> fonts;

    GameState currentState;

    Player player;
    std::mt19937 randomEngine;
    PlatformManager platformManager;
    UiManager uiManager;
    ScoreManager scoreManager;

    float previousPlayerBottom;
    sf::Clock frameClock;
};

#endif
