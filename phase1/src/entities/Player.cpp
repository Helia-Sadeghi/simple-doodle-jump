/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/Player.hpp"
#include "core/Constants.hpp"

Player::Player()
    : sprite(std::nullopt), leftTexture(nullptr), rightTexture(nullptr),
      position(0.f, 0.f), velocity(0.f, 0.f), facingRight(true) {
}

void Player::configure(const sf::Texture& leftTex, const sf::Texture& rightTex,
                        const sf::Vector2f& startPosition) {
    leftTexture = &leftTex;
    rightTexture = &rightTex;
    position = startPosition;
    velocity = sf::Vector2f(0.f, 0.f);
    facingRight = true;

    sprite.emplace(*rightTexture);
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    sprite->setPosition(position);
}

void Player::handleInput() {
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);

    if (left && !right) {
        velocity.x = -Constants::PLAYER_MOVE_SPEED;
        if (facingRight) {
            facingRight = false;
            sprite->setTexture(*leftTexture);
        }
    } else if (right && !left) {
        velocity.x = Constants::PLAYER_MOVE_SPEED;
        if (!facingRight) {
            facingRight = true;
            sprite->setTexture(*rightTexture);
        }
    } else {
        velocity.x = 0.f;
    }
}

void Player::update(float deltaTime, float windowWidth) {
    velocity.y += Constants::GRAVITY * deltaTime;

    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    sf::FloatRect bounds = sprite->getLocalBounds();
    float halfWidth = bounds.size.x / 2.f;
    if (position.x + halfWidth < 0.f) {
        position.x = windowWidth + halfWidth;
    } else if (position.x - halfWidth > windowWidth) {
        position.x = -halfWidth;
    }

    sprite->setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
    window.draw(*sprite);
}

void Player::applyVerticalBoost(float velocityY) {
    velocity.y = velocityY;
}

void Player::move(float dx, float dy) {
    position.x += dx;
    position.y += dy;
    sprite->setPosition(position);
}

sf::FloatRect Player::getBounds() const {
    return sprite->getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return position;
}

void Player::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    sprite->setPosition(position);
}

float Player::getVelocityY() const {
    return velocity.y;
}

void Player::setVelocityY(float vy) {
    velocity.y = vy;
}

void Player::reset(const sf::Vector2f& startPosition) {
    position = startPosition;
    velocity = sf::Vector2f(0.f, 0.f);
    facingRight = true;
    sprite->setTexture(*rightTexture);
    sprite->setPosition(position);
}
