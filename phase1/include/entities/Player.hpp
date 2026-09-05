/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <optional>

class Player {
private:
    std::optional<sf::Sprite> sprite;
    const sf::Texture* leftTexture;
    const sf::Texture* rightTexture;

    sf::Vector2f position;
    sf::Vector2f velocity;
    bool facingRight;

public:
    Player();

    void configure(const sf::Texture& leftTexture, const sf::Texture& rightTexture,
                    const sf::Vector2f& startPosition);

    void handleInput();
    void update(float deltaTime, float windowWidth);
    void render(sf::RenderWindow& window);

    void applyVerticalBoost(float velocityY);
    void move(float dx, float dy);

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& newPosition);

    float getVelocityY() const;
    void setVelocityY(float vy);

    void reset(const sf::Vector2f& startPosition);
};

#endif
