/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/NormalPlatform.hpp"

NormalPlatform::NormalPlatform(const sf::Texture& texture, const sf::Vector2f& startPosition)
    : Platform(texture, startPosition) {
}

PlatformType NormalPlatform::getType() const {
    return PlatformType::Normal;
}
