/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>

class Bullet {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    float velocityY;
    bool markedForRemoval;

public:
    explicit Bullet(const sf::Vector2f& startPosition);

    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    bool isOffScreen(float viewTopY) const;

    bool isMarkedForRemoval() const;
    void markForRemoval();
};

#endif
