/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/MovingPlatform.hpp"
#include <cmath>

MovingPlatform::MovingPlatform(const sf::Texture& texture, const sf::Vector2f& startPosition,
                                 float speed, float windowWidth)
    : Platform(texture, startPosition), speed(speed), windowWidth(windowWidth) {
    velocity.x = speed;
}

void MovingPlatform::update(float deltaTime) {
    float width = getSize().x;

    position.x += velocity.x * deltaTime;

    if (position.x <= 0.f) {
        position.x = 0.f;
        velocity.x = std::abs(velocity.x);
    } else if (position.x + width >= windowWidth) {
        position.x = windowWidth - width;
        velocity.x = -std::abs(velocity.x);
    }

    sprite.setPosition(position);
    if (spring) {
        spring->update(deltaTime);
        spring->updatePosition(position, getSize());
    }
}

PlatformType MovingPlatform::getType() const {
    return PlatformType::Moving;
}
