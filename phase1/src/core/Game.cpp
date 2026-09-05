/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include <ctime>

using Constants::WINDOW_WIDTH;
using Constants::WINDOW_HEIGHT;

Game::Game()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Simple Doodle Jump"),
      viewTopY(0.f),
      currentState(GameState::Menu),
      randomEngine(static_cast<unsigned int>(std::time(nullptr))),
      platformManager(textures, randomEngine),
      scoreManager(Constants::HIGHSCORE_FILE),
      previousPlayerBottom(0.f) {
    window.setFramerateLimit(Constants::FRAME_RATE_LIMIT);

    gameView.setSize({static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
    gameView.setCenter({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});

    loadResources();
    uiManager.setup(textures, fonts, WINDOW_WIDTH, WINDOW_HEIGHT);

    player.configure(textures.get("left_doodle"), textures.get("right_doodle"),
                      sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
}

void Game::loadResources() {
    textures.load("background", "assets/background.png");
    textures.load("start_button", "assets/start_button.png");
    textures.load("restart_button", "assets/restart_button.png");
    textures.load("menu_button", "assets/menu_button.png");
    textures.load("left_doodle", "assets/left_doodle.png");
    textures.load("right_doodle", "assets/right_doodle.png");
    textures.load("normal_platform", "assets/normal_platform.png");
    textures.load("moving_platform", "assets/moving_platform.png");
    textures.load("broken_platform", "assets/broken_platform.png");
    textures.load("spring", "assets/spring_sprite.png");

    fonts.load("main", "fonts/ariblk.ttf");
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = frameClock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePressed->position);
                handleMouseClick(worldPos);
            }
        }
    }
}

void Game::handleMouseClick(const sf::Vector2f& worldPos) {
    if (currentState == GameState::Menu) {
        if (uiManager.isStartClicked(worldPos)) {
            startNewGame();
        }
    } else if (currentState == GameState::GameOver) {
        if (uiManager.isRestartClicked(worldPos)) {
            startNewGame();
        } else if (uiManager.isMenuClicked(worldPos)) {
            currentState = GameState::Menu;
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == GameState::Gameplay) {
        updateGameplay(deltaTime);
    }
}

void Game::updateGameplay(float deltaTime) {
    player.handleInput();
    player.update(deltaTime, static_cast<float>(WINDOW_WIDTH));

    platformManager.update(deltaTime);
    platformManager.checkCollisions(player, previousPlayerBottom);

    float desiredViewTop = player.getPosition().y - Constants::SCROLL_THRESHOLD;
    if (desiredViewTop < viewTopY) {
        viewTopY = desiredViewTop;
        gameView.setCenter({WINDOW_WIDTH / 2.f, viewTopY + WINDOW_HEIGHT / 2.f});
    }
    scoreManager.updateFromHeight(-viewTopY);

    previousPlayerBottom = player.getBounds().position.y + player.getBounds().size.y;

    platformManager.maybeSpawnNew(viewTopY);
    platformManager.removeOffscreen(viewTopY);

    if (player.getPosition().y > viewTopY + WINDOW_HEIGHT) {
        triggerGameOver();
    }
}

void Game::startNewGame() {
    scoreManager.reset();
    viewTopY = 0.f;
    gameView.setCenter({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
    player.reset(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
    platformManager.spawnInitial();
    currentState = GameState::Gameplay;
}

void Game::triggerGameOver() {
    scoreManager.finalizeRun();
    currentState = GameState::GameOver;
}

void Game::render() {
    window.clear();
    window.setView(window.getDefaultView());
    uiManager.renderBackground(window);

    switch (currentState) {
        case GameState::Menu:
            uiManager.renderMenu(window, scoreManager.getBestScore());
            break;
        case GameState::Gameplay:
            window.setView(gameView);
            platformManager.render(window);
            player.render(window);
            window.setView(window.getDefaultView());
            uiManager.renderHud(window, scoreManager.getScore());
            break;
        case GameState::GameOver:
            uiManager.renderGameOver(window, scoreManager.getScore(), scoreManager.getBestScore());
            break;
    }

    window.display();
}
