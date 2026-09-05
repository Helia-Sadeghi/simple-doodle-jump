/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include <array>
#include "core/ResourceManager.hpp"
#include "core/Difficulty.hpp"

class UiManager {
public:
    void setup(ResourceManager<sf::Texture>& textures, ResourceManager<sf::Font>& fonts,
               unsigned int windowWidth, unsigned int windowHeight);

    void renderBackground(sf::RenderWindow& window);
    void renderMenu(sf::RenderWindow& window, float bestScore, Difficulty currentDifficulty);
    void renderHud(sf::RenderWindow& window, float score);
    void renderGameOver(sf::RenderWindow& window, float score, float bestScore);

    bool isStartClicked(const sf::Vector2f& point) const;
    bool isRestartClicked(const sf::Vector2f& point) const;
    bool isMenuClicked(const sf::Vector2f& point) const;

    // ── Phase 2: Settings screen (spec §5) ───────────────────────────────
    bool isSettingsClicked(const sf::Vector2f& point) const; // settings button on the main menu

    void renderSettings(sf::RenderWindow& window, float volume0to100, Difficulty currentDifficulty,
                         float bestScoreForDifficulty);
    bool isBackClicked(const sf::Vector2f& point) const;

    // Volume bar hit-testing: pass the click's x through to get 0..100.
    bool isVolumeBarClicked(const sf::Vector2f& point) const;
    float volumeFromClickX(float clickX) const;

    // Returns which difficulty button (if any) contains `point`.
    bool tryGetClickedDifficulty(const sf::Vector2f& point, Difficulty& outDifficulty) const;

private:
    unsigned int windowWidth;
    unsigned int windowHeight;

    std::optional<sf::Sprite> backgroundSprite;
    std::optional<sf::Sprite> startButtonSprite;
    std::optional<sf::Sprite> restartButtonSprite;
    std::optional<sf::Sprite> menuButtonSprite;

    std::optional<sf::Text> titleText;
    std::optional<sf::Text> bestScoreLabel;
    std::optional<sf::Text> modeLabel; // shows the currently selected difficulty on the main menu
    std::optional<sf::Text> scoreText;
    std::optional<sf::Text> gameOverText;
    std::optional<sf::Text> finalScoreText;
    std::optional<sf::Text> finalBestScoreText;

    // ── Phase 2 members ───────────────────────────────────────────────────
    std::optional<sf::Sprite> settingsButtonSprite;
    std::optional<sf::Sprite> backButtonSprite;

    std::optional<sf::Text> settingsTitleText;
    std::optional<sf::Text> settingsBestScoreText; // shows the record for whichever difficulty is selected
    std::optional<sf::Text> volumeValueText;

    sf::FloatRect volumeBarBounds;
    std::optional<sf::RectangleShape> volumeBarBackground;
    std::optional<sf::RectangleShape> volumeBarFill;
    std::optional<sf::CircleShape> volumeHandle;

    std::array<sf::FloatRect, 3> difficultyButtonBounds;
    std::array<std::optional<sf::RectangleShape>, 3> difficultyButtonShapes;
    std::array<std::optional<sf::Text>, 3> difficultyButtonTexts;
};

#endif
