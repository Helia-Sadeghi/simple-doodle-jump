/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/UiManager.hpp"
#include "core/Constants.hpp"
#include <sstream>

void UiManager::setup(ResourceManager<sf::Texture>& textures, ResourceManager<sf::Font>& fonts,
                       unsigned int width, unsigned int height) {
    windowWidth  = width;
    windowHeight = height;

    // ── Background ──────────────────────────────────────────────────────────
    backgroundSprite.emplace(textures.get("background"));
    sf::FloatRect bgBounds = backgroundSprite->getLocalBounds();
    backgroundSprite->setScale({windowWidth  / bgBounds.size.x,
                                 windowHeight / bgBounds.size.y});

    // ── Button sprites ───────────────────────────────────────────────────────
    startButtonSprite.emplace(textures.get("start_button"));
    restartButtonSprite.emplace(textures.get("restart_button"));
    menuButtonSprite.emplace(textures.get("menu_button"));

    auto centerOriginOf = [](sf::Sprite& sprite) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    };
    centerOriginOf(*startButtonSprite);
    centerOriginOf(*restartButtonSprite);
    centerOriginOf(*menuButtonSprite);

    startButtonSprite->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_START_BUTTON_Y_RATIO});
    restartButtonSprite->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_RESTART_BUTTON_Y_RATIO});
    menuButtonSprite->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_MENU_BUTTON_Y_RATIO});

    const sf::Font& font = fonts.get("main");

    // ── Title ────────────────────────────────────────────────────────────────
    titleText.emplace(font, "Doodle Jump", Constants::UI_TITLE_FONT_SIZE);
    titleText->setFillColor(sf::Color::White);
    titleText->setOutlineColor(sf::Color::Black);
    titleText->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);
    {
        sf::FloatRect b = titleText->getLocalBounds();
        titleText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    titleText->setPosition({windowWidth / 2.f, windowHeight * Constants::UI_TITLE_Y_RATIO});

    // ── Best-score label (menu screen) ───────────────────────────────────────
    bestScoreLabel.emplace(font, "", Constants::UI_BEST_SCORE_FONT_SIZE);
    bestScoreLabel->setFillColor(sf::Color::Yellow);
    bestScoreLabel->setOutlineColor(sf::Color::Black);
    bestScoreLabel->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);

    // ── Mode label (menu screen) — shows the currently selected difficulty ──
    modeLabel.emplace(font, "", Constants::UI_BEST_SCORE_FONT_SIZE);
    modeLabel->setOutlineColor(sf::Color::Black);
    modeLabel->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);

    // ── HUD score (gameplay) ─────────────────────────────────────────────────
    scoreText.emplace(font, "", Constants::UI_HUD_FONT_SIZE);
    scoreText->setFillColor(sf::Color::White);
    scoreText->setOutlineColor(sf::Color::Black);
    scoreText->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);
    scoreText->setPosition({Constants::UI_SCORE_TEXT_X, Constants::UI_SCORE_TEXT_Y});

    // ── "YOU LOST" header ─────────────────────────────────────────────────────
    gameOverText.emplace(font, "YOU LOST", Constants::UI_GAME_OVER_FONT_SIZE);
    gameOverText->setFillColor(sf::Color::Red);
    gameOverText->setOutlineColor(sf::Color::Black);
    gameOverText->setOutlineThickness(Constants::UI_GAMEOVER_OUTLINE_THICKNESS);
    {
        sf::FloatRect b = gameOverText->getLocalBounds();
        gameOverText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    gameOverText->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_GAME_OVER_TEXT_Y_RATIO});

    // ── Game-over score lines ────────────────────────────────────────────────
    finalScoreText.emplace(font, "", Constants::UI_HUD_FONT_SIZE);
    finalScoreText->setFillColor(sf::Color::White);
    finalScoreText->setOutlineColor(sf::Color::Black);
    finalScoreText->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);

    finalBestScoreText.emplace(font, "", Constants::UI_HUD_FONT_SIZE);
    finalBestScoreText->setFillColor(sf::Color::Yellow);
    finalBestScoreText->setOutlineColor(sf::Color::Black);
    finalBestScoreText->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);

    // ══════════════════════════════════════════════════════════════════
    // ─── Phase 2: Settings screen (spec §5) ─────────────────────────────
    // ══════════════════════════════════════════════════════════════════

    settingsButtonSprite.emplace(textures.get("settings_button"));
    centerOriginOf(*settingsButtonSprite);
    settingsButtonSprite->setScale({Constants::UI_SETTINGS_BUTTON_SCALE, Constants::UI_SETTINGS_BUTTON_SCALE});
    settingsButtonSprite->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_SETTINGS_BUTTON_Y_RATIO});

    backButtonSprite.emplace(textures.get("back_button"));
    centerOriginOf(*backButtonSprite);
    backButtonSprite->setScale({Constants::UI_BACK_BUTTON_SCALE, Constants::UI_BACK_BUTTON_SCALE});
    backButtonSprite->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_BACK_BUTTON_Y_RATIO});

    settingsTitleText.emplace(font, "Settings", Constants::UI_TITLE_FONT_SIZE);
    settingsTitleText->setFillColor(sf::Color::White);
    settingsTitleText->setOutlineColor(sf::Color::Black);
    settingsTitleText->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);
    {
        sf::FloatRect b = settingsTitleText->getLocalBounds();
        settingsTitleText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    settingsTitleText->setPosition({windowWidth / 2.f, windowHeight * Constants::UI_TITLE_Y_RATIO});

    settingsBestScoreText.emplace(font, "", Constants::UI_HUD_FONT_SIZE);
    settingsBestScoreText->setFillColor(sf::Color::White);
    settingsBestScoreText->setOutlineColor(sf::Color::Black);
    settingsBestScoreText->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);

    volumeValueText.emplace(font, "", Constants::UI_HUD_FONT_SIZE);
    volumeValueText->setFillColor(sf::Color::White);
    volumeValueText->setOutlineColor(sf::Color::Black);
    volumeValueText->setOutlineThickness(Constants::UI_TEXT_OUTLINE_THICKNESS);

    float barX = windowWidth / 2.f - Constants::UI_VOLUME_BAR_WIDTH / 2.f;
    float barY = windowHeight * Constants::UI_VOLUME_BAR_Y_RATIO;
    volumeBarBounds = sf::FloatRect({barX, barY - 12.f},
                                     {Constants::UI_VOLUME_BAR_WIDTH, Constants::UI_VOLUME_BAR_HEIGHT + 24.f});

    volumeBarBackground.emplace(sf::Vector2f{Constants::UI_VOLUME_BAR_WIDTH, Constants::UI_VOLUME_BAR_HEIGHT});
    volumeBarBackground->setPosition({barX, barY});
    volumeBarBackground->setFillColor(sf::Color(60, 60, 60));
    volumeBarBackground->setOutlineColor(sf::Color::Black);
    volumeBarBackground->setOutlineThickness(2.f);

    volumeBarFill.emplace(sf::Vector2f{0.f, Constants::UI_VOLUME_BAR_HEIGHT});
    volumeBarFill->setPosition({barX, barY});
    volumeBarFill->setFillColor(sf::Color::Yellow);

    volumeHandle.emplace(Constants::UI_VOLUME_BAR_HEIGHT);
    volumeHandle->setOrigin({Constants::UI_VOLUME_BAR_HEIGHT / 2.f, Constants::UI_VOLUME_BAR_HEIGHT / 2.f});
    volumeHandle->setFillColor(sf::Color::White);
    volumeHandle->setOutlineColor(sf::Color::Black);
    volumeHandle->setOutlineThickness(1.f);

    const char* difficultyLabels[3] = {"Easy", "Medium", "Hard"};
    for (int i = 0; i < 3; ++i) {
        float centerX = windowWidth / 2.f + (static_cast<float>(i) - 1.f) * Constants::UI_DIFFICULTY_BUTTON_GAP;
        float centerY = windowHeight * Constants::UI_DIFFICULTY_Y_RATIO;
        float size = Constants::UI_DIFFICULTY_BUTTON_SIZE;

        difficultyButtonBounds[i] = sf::FloatRect(
            {centerX - size / 2.f, centerY - size / 2.f}, {size, size});

        difficultyButtonShapes[i].emplace(sf::Vector2f{size, size});
        difficultyButtonShapes[i]->setPosition({centerX - size / 2.f, centerY - size / 2.f});
        difficultyButtonShapes[i]->setFillColor(sf::Color(60, 60, 60));
        difficultyButtonShapes[i]->setOutlineColor(sf::Color::White);
        difficultyButtonShapes[i]->setOutlineThickness(2.f);

        difficultyButtonTexts[i].emplace(font, difficultyLabels[i], 16);
        difficultyButtonTexts[i]->setFillColor(sf::Color::White);
        difficultyButtonTexts[i]->setOutlineColor(sf::Color::Black);
        difficultyButtonTexts[i]->setOutlineThickness(1.f);
        {
            sf::FloatRect b = difficultyButtonTexts[i]->getLocalBounds();
            difficultyButtonTexts[i]->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        }
        difficultyButtonTexts[i]->setPosition({centerX, centerY});
    }
}

void UiManager::renderBackground(sf::RenderWindow& window) {
    window.draw(*backgroundSprite);
}

void UiManager::renderMenu(sf::RenderWindow& window, float bestScore, Difficulty currentDifficulty) {
    window.draw(*titleText);

    std::ostringstream oss;
    oss << "Best: " << static_cast<int>(bestScore);
    bestScoreLabel->setString(oss.str());
    {
        sf::FloatRect b = bestScoreLabel->getLocalBounds();
        bestScoreLabel->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    bestScoreLabel->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_BEST_SCORE_Y_RATIO});
    window.draw(*bestScoreLabel);

    // Tells the player which mode they're about to play before they hit
    // Start, so they're never surprised by what Easy/Medium/Hard means.
    static const char* MODE_NAMES[3] = {"Easy", "Medium", "Hard"};
    static const sf::Color MODE_COLORS[3] = {
        sf::Color(120, 220, 120), sf::Color(240, 200, 60), sf::Color(230, 90, 90)};
    int modeIndex = static_cast<int>(currentDifficulty);
    std::ostringstream modeOss;
    modeOss << "Mode: " << MODE_NAMES[modeIndex];
    modeLabel->setString(modeOss.str());
    modeLabel->setFillColor(MODE_COLORS[modeIndex]);
    {
        sf::FloatRect b = modeLabel->getLocalBounds();
        modeLabel->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    modeLabel->setPosition({windowWidth / 2.f, windowHeight * Constants::UI_MODE_TEXT_Y_RATIO});
    window.draw(*modeLabel);

    window.draw(*startButtonSprite);
    window.draw(*settingsButtonSprite);
}

void UiManager::renderHud(sf::RenderWindow& window, float score) {
    std::ostringstream oss;
    oss << "Score: " << static_cast<int>(score);
    scoreText->setString(oss.str());
    window.draw(*scoreText);
}

void UiManager::renderGameOver(sf::RenderWindow& window, float score, float bestScore) {
    window.draw(*gameOverText);

    std::ostringstream finalOss;
    finalOss << "Score: " << static_cast<int>(score);
    finalScoreText->setString(finalOss.str());
    {
        sf::FloatRect b = finalScoreText->getLocalBounds();
        finalScoreText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    finalScoreText->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_FINAL_SCORE_Y_RATIO});
    window.draw(*finalScoreText);

    std::ostringstream bestOss;
    bestOss << "Best: " << static_cast<int>(bestScore);
    finalBestScoreText->setString(bestOss.str());
    {
        sf::FloatRect b = finalBestScoreText->getLocalBounds();
        finalBestScoreText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    finalBestScoreText->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_FINAL_BEST_Y_RATIO});
    window.draw(*finalBestScoreText);

    window.draw(*restartButtonSprite);
    window.draw(*menuButtonSprite);
}

bool UiManager::isStartClicked(const sf::Vector2f& point) const {
    return startButtonSprite->getGlobalBounds().contains(point);
}

bool UiManager::isRestartClicked(const sf::Vector2f& point) const {
    return restartButtonSprite->getGlobalBounds().contains(point);
}

bool UiManager::isMenuClicked(const sf::Vector2f& point) const {
    return menuButtonSprite->getGlobalBounds().contains(point);
}

// ══════════════════════════════════════════════════════════════════════
// ─── Phase 2: Settings screen (spec §5) ─────────────────────────────────
// ══════════════════════════════════════════════════════════════════════

bool UiManager::isSettingsClicked(const sf::Vector2f& point) const {
    return settingsButtonSprite->getGlobalBounds().contains(point);
}

bool UiManager::isBackClicked(const sf::Vector2f& point) const {
    return backButtonSprite->getGlobalBounds().contains(point);
}

bool UiManager::isVolumeBarClicked(const sf::Vector2f& point) const {
    return volumeBarBounds.contains(point);
}

float UiManager::volumeFromClickX(float clickX) const {
    float barX = windowWidth / 2.f - Constants::UI_VOLUME_BAR_WIDTH / 2.f;
    float ratio = (clickX - barX) / Constants::UI_VOLUME_BAR_WIDTH;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;
    return ratio * 100.f;
}

bool UiManager::tryGetClickedDifficulty(const sf::Vector2f& point, Difficulty& outDifficulty) const {
    for (int i = 0; i < 3; ++i) {
        if (difficultyButtonBounds[i].contains(point)) {
            outDifficulty = static_cast<Difficulty>(i);
            return true;
        }
    }
    return false;
}

void UiManager::renderSettings(sf::RenderWindow& window, float volume0to100, Difficulty currentDifficulty,
                                float bestScoreForDifficulty) {
    window.draw(*settingsTitleText);

    // Bug fix (spec feedback): the record for whichever difficulty is
    // currently selected must be visible right here, not only back on the
    // main menu — updates live the instant a difficulty button is clicked.
    static const char* NAMES[3] = {"Easy", "Medium", "Hard"};
    std::ostringstream bestOss;
    bestOss << NAMES[static_cast<int>(currentDifficulty)] << " Best: "
            << static_cast<int>(bestScoreForDifficulty);
    settingsBestScoreText->setString(bestOss.str());
    {
        sf::FloatRect b = settingsBestScoreText->getLocalBounds();
        settingsBestScoreText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    settingsBestScoreText->setPosition(
        {windowWidth / 2.f, windowHeight * Constants::UI_SETTINGS_BEST_SCORE_Y_RATIO});
    window.draw(*settingsBestScoreText);

    // ── Volume bar ──────────────────────────────────────────────────────
    window.draw(*volumeBarBackground);

    float ratio = volume0to100 / 100.f;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;

    volumeBarFill->setSize({Constants::UI_VOLUME_BAR_WIDTH * ratio, Constants::UI_VOLUME_BAR_HEIGHT});
    window.draw(*volumeBarFill);

    float barX = windowWidth / 2.f - Constants::UI_VOLUME_BAR_WIDTH / 2.f;
    float barY = windowHeight * Constants::UI_VOLUME_BAR_Y_RATIO;
    volumeHandle->setPosition({barX + Constants::UI_VOLUME_BAR_WIDTH * ratio,
                                barY + Constants::UI_VOLUME_BAR_HEIGHT / 2.f});
    window.draw(*volumeHandle);

    std::ostringstream volOss;
    volOss << "Volume: " << static_cast<int>(volume0to100) << "%";
    volumeValueText->setString(volOss.str());
    {
        sf::FloatRect b = volumeValueText->getLocalBounds();
        volumeValueText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
    }
    volumeValueText->setPosition({windowWidth / 2.f, barY - 26.f});
    window.draw(*volumeValueText);

    // ── Difficulty buttons ──────────────────────────────────────────────
    for (int i = 0; i < 3; ++i) {
        bool selected = (static_cast<Difficulty>(i) == currentDifficulty);
        difficultyButtonShapes[i]->setFillColor(selected ? sf::Color(70, 130, 70) : sf::Color(60, 60, 60));
        difficultyButtonShapes[i]->setOutlineColor(selected ? sf::Color::Yellow : sf::Color::White);
        window.draw(*difficultyButtonShapes[i]);
        window.draw(*difficultyButtonTexts[i]);
    }

    window.draw(*backButtonSprite);
}
