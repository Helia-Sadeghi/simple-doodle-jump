/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/BreakablePlatform.hpp"
#include "core/Constants.hpp"

BreakablePlatform::BreakablePlatform(const sf::Texture& texture, const sf::Vector2f& startPosition)
    : Platform(texture, startPosition), broken(false), fallSpeed(0.f) {
}

void BreakablePlatform::update(float deltaTime) {
    if (broken) {
        fallSpeed += Constants::GRAVITY * deltaTime;
        position.y += fallSpeed * deltaTime;
        sprite.setPosition(position);
    }
}

float BreakablePlatform::onPlayerLand() {
    broken = true;
    fallSpeed = 0.f;
    return 0.f;
}

bool BreakablePlatform::canJumpOn() const {
    return !broken;
}

PlatformType BreakablePlatform::getType() const {
    return PlatformType::Breakable;
}

bool BreakablePlatform::isBroken() const {
    return broken;
}
