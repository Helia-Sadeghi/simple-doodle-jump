/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef BREAKABLE_PLATFORM_HPP
#define BREAKABLE_PLATFORM_HPP

#include "entities/Platform.hpp"

class BreakablePlatform : public Platform {
private:
    bool broken;
    float fallSpeed;

public:
    BreakablePlatform(const sf::Texture& texture, const sf::Vector2f& startPosition);

    void update(float deltaTime) override;
    float onPlayerLand() override;
    bool canJumpOn() const override;
    PlatformType getType() const override;

    bool isBroken() const;
};

#endif
