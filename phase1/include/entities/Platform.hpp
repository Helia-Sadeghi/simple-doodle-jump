/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "entities/Spring.hpp"

enum class PlatformType {
    Normal,
    Moving,
    Breakable
};

class Platform {
protected:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool markedForRemoval;
    std::unique_ptr<Spring> spring;

public:
    Platform(const sf::Texture& texture, const sf::Vector2f& startPosition);
    virtual ~Platform() = default;

    Platform(const Platform&) = delete;
    Platform& operator=(const Platform&) = delete;

    virtual void update(float deltaTime);
    virtual void render(sf::RenderWindow& window);
    virtual float onPlayerLand();
    virtual bool canJumpOn() const;
    virtual PlatformType getType() const = 0;

    void move(float dx, float dy);
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& newPosition);
    sf::FloatRect getBounds() const;
    sf::Vector2f getSize() const;

    void attachSpring(std::unique_ptr<Spring> newSpring);
    bool hasSpring() const;
    Spring* getSpring() const;

    bool isMarkedForRemoval() const;
    void markForRemoval();
};

#endif
