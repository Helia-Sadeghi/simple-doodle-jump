/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef SPRING_HPP
#define SPRING_HPP

#include <SFML/Graphics.hpp>

class Spring {
private:
    sf::Sprite sprite;
    sf::IntRect relaxedFrame;
    sf::IntRect compressedFrame;
    float compressedTimeRemaining;

    void refreshOrigin();

public:
    explicit Spring(const sf::Texture& texture);

    void updatePosition(const sf::Vector2f& platformPosition, const sf::Vector2f& platformSize);
    void update(float deltaTime);
    void trigger();
    void render(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    float getBoostVelocity() const;
};

#endif
