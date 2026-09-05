/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef NORMAL_PLATFORM_HPP
#define NORMAL_PLATFORM_HPP

#include "entities/Platform.hpp"

class NormalPlatform : public Platform {
public:
    NormalPlatform(const sf::Texture& texture, const sf::Vector2f& startPosition);
    PlatformType getType() const override;
};

#endif
