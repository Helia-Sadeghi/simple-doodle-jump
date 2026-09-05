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
#include <algorithm>

using Constants::WINDOW_WIDTH;
using Constants::WINDOW_HEIGHT;

Game::Game()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Simple Doodle Jump"),
      viewTopY(0.f),
      currentState(GameState::Menu),
      stateBeforeSettings(GameState::Menu),
      randomEngine(static_cast<unsigned int>(std::time(nullptr))),
      platformManager(textures, randomEngine),
      scoreManager(Constants::HIGHSCORE_FILE),
      previousPlayerBottom(0.f),
      settingsManager(Constants::SETTINGS_FILE),
      monsterManager(textures, randomEngine),
      holeManager(textures, randomEngine),
      comboManager(fonts),
      firePressed(false),
      timeSinceLastShot(0.f),
      runElapsedTime(0.f),
      isBeingSucked(false),
      suckTimer(0.f),
      draggingVolume(false) {
    window.setFramerateLimit(Constants::FRAME_RATE_LIMIT);

    gameView.setSize({static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)});
    gameView.setCenter({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});

    loadResources();
    uiManager.setup(textures, fonts, WINDOW_WIDTH, WINDOW_HEIGHT);

    player.configure(textures.get("left_doodle"), textures.get("right_doodle"),
                      sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
    player.configureShooting(textures.get("shooting_pose"));
    player.configureNose(textures.get("nose"));

    // Phase 2: apply whatever was saved in settings.txt last time.
    soundManager.setVolume(settingsManager.getVolume());
    scoreManager.setDifficulty(settingsManager.getDifficulty());

    soundManager.playMusic(); // menu music (spec §4.1)
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

    // ── Phase 2 assets ──────────────────────────────────────────────────
    textures.load("shooting_pose", "assets/Shooting@Pose.png");
    textures.load("nose", "assets/Nose.png");
    textures.load("monster_blue", "assets/BlueMonster.png");
    textures.load("monster_green", "assets/green_monster.png");
    textures.load("hole", "assets/hole.png");
    textures.load("settings_button", "assets/Settings_button.png");
    textures.load("back_button", "assets/back_button.png");

    soundManager.loadResources();
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
                if (currentState == GameState::Settings && uiManager.isVolumeBarClicked(worldPos)) {
                    draggingVolume = true;
                }
            }
        } else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseReleased->button == sf::Mouse::Button::Left) {
                draggingVolume = false;
            }
        } else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            if (draggingVolume && currentState == GameState::Settings) {
                sf::Vector2f worldPos = window.mapPixelToCoords(mouseMoved->position);
                float newVolume = uiManager.volumeFromClickX(worldPos.x);
                settingsManager.setVolume(newVolume);
                soundManager.setVolume(newVolume);
            }
        }
    }
}

void Game::handleMouseClick(const sf::Vector2f& worldPos) {
    if (currentState == GameState::Menu) {
        if (uiManager.isStartClicked(worldPos)) {
            startNewGame();
        } else if (uiManager.isSettingsClicked(worldPos)) {
            // Settings can only be opened from the main menu (spec §5).
            stateBeforeSettings = currentState;
            currentState = GameState::Settings;
        }
    } else if (currentState == GameState::GameOver) {
        if (uiManager.isRestartClicked(worldPos)) {
            startNewGame();
        } else if (uiManager.isMenuClicked(worldPos)) {
            currentState = GameState::Menu;
            scoreManager.setDifficulty(settingsManager.getDifficulty());
            soundManager.playMusic();
        }
    } else if (currentState == GameState::Settings) {
        if (uiManager.isBackClicked(worldPos)) {
            settingsManager.save();
            currentState = stateBeforeSettings;
        } else if (uiManager.isVolumeBarClicked(worldPos)) {
            float newVolume = uiManager.volumeFromClickX(worldPos.x);
            settingsManager.setVolume(newVolume);
            soundManager.setVolume(newVolume);
        } else {
            Difficulty clickedDifficulty;
            if (uiManager.tryGetClickedDifficulty(worldPos, clickedDifficulty)) {
                settingsManager.setDifficulty(clickedDifficulty);
                scoreManager.setDifficulty(clickedDifficulty);
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == GameState::Gameplay) {
        updateGameplay(deltaTime);
    }
}

void Game::updateGameplay(float deltaTime) {
    // While the player is being sucked into a hole, everything else in the
    // world is frozen (spec §6.2, point 1) — only the shrink animation runs.
    if (isBeingSucked) {
        updateSuckSequence(deltaTime);
        return;
    }

    runElapsedTime += deltaTime;

    player.handleInput();
    player.update(deltaTime, static_cast<float>(WINDOW_WIDTH));

    const Hole* hitHole = holeManager.checkPlayerCollision(player.getBounds());
    if (hitHole != nullptr) {
        isBeingSucked = true;
        suckStartPosition = player.getPosition();
        suckTargetPosition = hitHole->getCenter();
        suckTimer = 0.f;
        soundManager.playLose(); // the "strange sound" plays as the fall begins, not after
        return;
    }

    platformManager.update(deltaTime);
    platformManager.checkCollisions(player, previousPlayerBottom, &soundManager);

    MonsterHitResult monsterResult =
        monsterManager.checkPlayerCollision(player.getBounds(), player.getVelocityY(), previousPlayerBottom);
    if (monsterResult == MonsterHitResult::Bounce) {
        player.applyVerticalBoost(Constants::MONSTER_LAND_BOOST_VELOCITY);
    } else if (monsterResult == MonsterHitResult::Kill) {
        triggerGameOver();
        return;
    }

    monsterManager.update(deltaTime);
    monsterManager.removeOffscreen(viewTopY);

    updateShooting(deltaTime);
    bulletManager.update(deltaTime, viewTopY);


    std::vector<sf::Vector2f> killPositions = monsterManager.checkBulletCollisions(bulletManager.getBullets());
    for (const sf::Vector2f& killPosition : killPositions) {
        scoreManager.addBonus(comboManager.registerKill(killPosition));
    }
    comboManager.update(deltaTime);

    float desiredViewTop = player.getPosition().y - Constants::SCROLL_THRESHOLD;
    if (desiredViewTop < viewTopY) {
        viewTopY = desiredViewTop;
        gameView.setCenter({WINDOW_WIDTH / 2.f, viewTopY + WINDOW_HEIGHT / 2.f});
    }
    scoreManager.updateFromHeight(-viewTopY);

    previousPlayerBottom = player.getBounds().position.y + player.getBounds().size.y;

    platformManager.maybeSpawnNew(viewTopY);
    platformManager.removeOffscreen(viewTopY);


    std::vector<sf::FloatRect> platformBounds = platformManager.getAllBounds();
    std::vector<sf::FloatRect> holeBounds     = holeManager.getAllBounds();
    std::vector<sf::FloatRect> monsterBounds  = monsterManager.getAllBounds();

    std::vector<sf::FloatRect> forMonsterSpawn = platformBounds;
    forMonsterSpawn.insert(forMonsterSpawn.end(), holeBounds.begin(), holeBounds.end());
    monsterManager.maybeSpawnNew(platformManager.getPlatforms(), forMonsterSpawn, -viewTopY);

    monsterBounds = monsterManager.getAllBounds();

    std::vector<sf::FloatRect> forHoleSpawn = platformBounds;
    forHoleSpawn.insert(forHoleSpawn.end(), monsterBounds.begin(), monsterBounds.end());
    holeManager.maybeSpawnNew(platformManager.getPlatforms(), forHoleSpawn, -viewTopY);

    if (player.getPosition().y > viewTopY + WINDOW_HEIGHT) {
        triggerGameOver();
    }
}

void Game::updateShooting(float deltaTime) {
    firePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    player.setShooting(firePressed);

    if (!firePressed) {
        // Ready to fire again the instant the key is pressed (spec §3.1).
        timeSinceLastShot = currentFireRate();
        return;
    }

    timeSinceLastShot += deltaTime;
    if (timeSinceLastShot >= currentFireRate()) {
        timeSinceLastShot = 0.f;
        bulletManager.spawn(player.getBulletSpawnPosition());
        soundManager.playShoot();
    }
}

float Game::currentFireRate() const {
    switch (settingsManager.getDifficulty()) {
        case Difficulty::Easy:   return Constants::FIRE_RATE_EASY;
        case Difficulty::Medium: return Constants::FIRE_RATE_MEDIUM;
        case Difficulty::Hard:
        default:                 return Constants::FIRE_RATE_HARD;
    }
}

void Game::updateSuckSequence(float deltaTime) {
    suckTimer += deltaTime;
    float t = suckTimer / Constants::HOLE_SUCK_DURATION;
    if (t > 1.f) {
        t = 1.f;
    }

    sf::Vector2f newPosition(
        suckStartPosition.x + (suckTargetPosition.x - suckStartPosition.x) * t,
        suckStartPosition.y + (suckTargetPosition.y - suckStartPosition.y) * t);
    player.setPosition(newPosition);
    player.setShrinkScale(1.f - t);

    if (t >= 1.f) {
        triggerGameOver(false); // lose sound already played when the fall began
    }
}

void Game::startNewGame() {
    Difficulty difficulty = settingsManager.getDifficulty();
    scoreManager.setDifficulty(difficulty);
    scoreManager.reset();

    viewTopY = 0.f;
    gameView.setCenter({WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f});
    player.reset(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));

    float speedMultiplier = Constants::MOVING_PLATFORM_SPEED_MULT_EASY;
    if (difficulty == Difficulty::Medium) {
        speedMultiplier = Constants::MOVING_PLATFORM_SPEED_MULT_MEDIUM;
    } else if (difficulty == Difficulty::Hard) {
        speedMultiplier = Constants::MOVING_PLATFORM_SPEED_MULT_HARD;
    }
    platformManager.setSpeedMultiplier(speedMultiplier);
    platformManager.spawnInitial();

    bulletManager.clear();
    monsterManager.reset(difficulty);
    holeManager.reset(difficulty);
    comboManager.reset();

    firePressed = false;
    timeSinceLastShot = 0.f;
    runElapsedTime = 0.f;
    isBeingSucked = false;
    suckTimer = 0.f;

    soundManager.stopMusic(); // no music during gameplay (spec §4.1)

    currentState = GameState::Gameplay;
}

void Game::triggerGameOver(bool playLoseSound) {
    if (playLoseSound) {
        soundManager.playLose();
    }
    scoreManager.finalizeRun();
    currentState = GameState::GameOver;
}

void Game::render() {
    window.clear();
    window.setView(window.getDefaultView());
    uiManager.renderBackground(window);

    switch (currentState) {
        case GameState::Menu:
            uiManager.renderMenu(window, scoreManager.getBestScore(), settingsManager.getDifficulty());
            break;
        case GameState::Settings:
            uiManager.renderSettings(window, settingsManager.getVolume(), settingsManager.getDifficulty(),
                                      scoreManager.getBestScore());
            break;
        case GameState::Gameplay:
            window.setView(gameView);
            platformManager.render(window);
            monsterManager.render(window);
            holeManager.render(window);
            bulletManager.render(window);
            comboManager.render(window);
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
