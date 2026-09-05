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
#include "core/ResourceManager.hpp"

class UiManager {
public:
    void setup(ResourceManager<sf::Texture>& textures, ResourceManager<sf::Font>& fonts,
               unsigned int windowWidth, unsigned int windowHeight);

    void renderBackground(sf::RenderWindow& window);
    void renderMenu(sf::RenderWindow& window, float bestScore);
    void renderHud(sf::RenderWindow& window, float score);
    void renderGameOver(sf::RenderWindow& window, float score, float bestScore);

    bool isStartClicked(const sf::Vector2f& point) const;
    bool isRestartClicked(const sf::Vector2f& point) const;
    bool isMenuClicked(const sf::Vector2f& point) const;

private:
    unsigned int windowWidth;
    unsigned int windowHeight;

    std::optional<sf::Sprite> backgroundSprite;
    std::optional<sf::Sprite> startButtonSprite;
    std::optional<sf::Sprite> restartButtonSprite;
    std::optional<sf::Sprite> menuButtonSprite;

    std::optional<sf::Text> titleText;
    std::optional<sf::Text> bestScoreLabel;
    std::optional<sf::Text> scoreText;
    std::optional<sf::Text> gameOverText;
    std::optional<sf::Text> finalScoreText;
    std::optional<sf::Text> finalBestScoreText;
};

#endif
