/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/Platform.hpp"
#include "core/Constants.hpp"

Platform::Platform(const sf::Texture& texture, const sf::Vector2f& startPosition)
    : sprite(texture), position(startPosition), velocity(0.f, 0.f),
      markedForRemoval(false), spring(nullptr) {
    sprite.setPosition(position);
}

void Platform::update(float deltaTime) {
    sprite.setPosition(position);
    if (spring) {
        spring->update(deltaTime);
        spring->updatePosition(position, getSize());
    }
}

void Platform::render(sf::RenderWindow& window) {
    window.draw(sprite);
    if (spring) {
        spring->render(window);
    }
}

float Platform::onPlayerLand() {
    return Constants::PLAYER_JUMP_VELOCITY;
}

bool Platform::canJumpOn() const {
    return true;
}

void Platform::move(float dx, float dy) {
    position.x += dx;
    position.y += dy;
    sprite.setPosition(position);
    if (spring) {
        spring->updatePosition(position, getSize());
    }
}

sf::Vector2f Platform::getPosition() const {
    return position;
}

void Platform::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite.setPosition(position);
    if (spring) {
        spring->updatePosition(position, getSize());
    }
}

sf::FloatRect Platform::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Platform::getSize() const {
    sf::FloatRect bounds = sprite.getLocalBounds();
    return bounds.size;
}

void Platform::attachSpring(std::unique_ptr<Spring> newSpring) {
    spring = std::move(newSpring);
    if (spring) {
        spring->updatePosition(position, getSize());
    }
}

bool Platform::hasSpring() const {
    return spring != nullptr;
}

Spring* Platform::getSpring() const {
    return spring.get();
}

bool Platform::isMarkedForRemoval() const {
    return markedForRemoval;
}

void Platform::markForRemoval() {
    markedForRemoval = true;
}
