/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/Bullet.hpp"
#include "core/Constants.hpp"
#include <array>
#include <random>

namespace {
// A little candy-colored palette — every shot gets a random fun color
// instead of a single flat one.
const std::array<sf::Color, 6> BULLET_COLOR_PALETTE = {
    sf::Color(255, 99, 132),  // watermelon pink
    sf::Color(54, 162, 235),  // sky blue
    sf::Color(255, 206, 86),  // lemon yellow
    sf::Color(75, 192, 145),  // minty teal
    sf::Color(153, 102, 255), // grape purple
    sf::Color(255, 159, 64)   // tangerine orange
};

sf::Color randomBulletColor() {
    static std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<std::size_t> distribution(0, BULLET_COLOR_PALETTE.size() - 1);
    return BULLET_COLOR_PALETTE[distribution(engine)];
}
}

Bullet::Bullet(const sf::Vector2f& startPosition)
    : shape(Constants::BULLET_RADIUS), position(startPosition),
      velocityY(Constants::BULLET_SPEED), markedForRemoval(false) {
    shape.setOrigin({Constants::BULLET_RADIUS, Constants::BULLET_RADIUS});
    shape.setFillColor(randomBulletColor());
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(1.5f);
    shape.setPosition(position);
}

void Bullet::update(float deltaTime) {
    position.y += velocityY * deltaTime;
    shape.setPosition(position);
}

void Bullet::render(sf::RenderWindow& window) const {
    window.draw(shape);
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Bullet::getPosition() const {
    return position;
}

bool Bullet::isOffScreen(float viewTopY) const {
    return (position.y + Constants::BULLET_RADIUS) < viewTopY;
}

bool Bullet::isMarkedForRemoval() const {
    return markedForRemoval;
}

void Bullet::markForRemoval() {
    markedForRemoval = true;
}
