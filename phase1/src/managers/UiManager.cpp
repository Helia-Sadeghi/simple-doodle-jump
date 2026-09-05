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
}

void UiManager::renderBackground(sf::RenderWindow& window) {
    window.draw(*backgroundSprite);
}

void UiManager::renderMenu(sf::RenderWindow& window, float bestScore) {
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

    window.draw(*startButtonSprite);
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
