/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/Monster.hpp"
#include <cmath>
#include <algorithm>

Monster::Monster(const sf::Texture& texture, const sf::Vector2f& startPosition,
                  float speed, float minX, float maxX, int health)
    : sprite(texture), position(startPosition), velocity(speed, 0.f),
      patrolMinX(std::min(minX, maxX)), patrolMaxX(std::max(minX, maxX)),
      health(health), markedForRemoval(false) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    sprite.setPosition(position);
}

void Monster::update(float deltaTime) {
    position.x += velocity.x * deltaTime;

    if (position.x <= patrolMinX) {
        position.x = patrolMinX;
        velocity.x = std::abs(velocity.x);
    } else if (position.x >= patrolMaxX) {
        position.x = patrolMaxX;
        velocity.x = -std::abs(velocity.x);
    }

    sprite.setPosition(position);
}

void Monster::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Monster::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        health = 0;
        markedForRemoval = true;
        return true;
    }
    return false;
}

bool Monster::isDead() const {
    return health <= 0;
}

sf::FloatRect Monster::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Monster::getPosition() const {
    return position;
}

sf::Vector2f Monster::getSize() const {
    return sprite.getLocalBounds().size;
}

void Monster::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
}

bool Monster::isMarkedForRemoval() const {
    return markedForRemoval;
}

void Monster::markForRemoval() {
    markedForRemoval = true;
}
