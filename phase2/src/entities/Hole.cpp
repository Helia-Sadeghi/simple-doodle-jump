/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/Hole.hpp"

Hole::Hole(const sf::Texture& texture, const sf::Vector2f& startPosition, float scale)
    : sprite(texture), position(startPosition), markedForRemoval(false) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    sprite.setScale({scale, scale});
    sprite.setPosition(position);
}

void Hole::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Hole::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Hole::getCenter() const {
    return position;
}

sf::Vector2f Hole::getPosition() const {
    return position;
}

sf::Vector2f Hole::getSize() const {
    return getBounds().size;
}

bool Hole::isMarkedForRemoval() const {
    return markedForRemoval;
}

void Hole::markForRemoval() {
    markedForRemoval = true;
}
