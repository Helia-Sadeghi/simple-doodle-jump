/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>

class Monster {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;

    float patrolMinX;
    float patrolMaxX;

    int health;
    bool markedForRemoval;

public:
    Monster(const sf::Texture& texture, const sf::Vector2f& startPosition,
            float speed, float patrolMinX, float patrolMaxX, int health);

    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;

    // Called when a bullet hits it. Returns true once its health reaches 0.
    bool takeDamage(int amount = 1);
    bool isDead() const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;
    void setPosition(const sf::Vector2f& newPosition);

    bool isMarkedForRemoval() const;
    void markForRemoval();
};

#endif
