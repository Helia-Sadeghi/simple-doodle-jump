/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef MOVING_PLATFORM_HPP
#define MOVING_PLATFORM_HPP

#include "entities/Platform.hpp"

class MovingPlatform : public Platform {
private:
    float speed;
    float windowWidth;

public:
    MovingPlatform(const sf::Texture& texture, const sf::Vector2f& startPosition,
                    float speed, float windowWidth);

    void update(float deltaTime) override;
    PlatformType getType() const override;
};

#endif
