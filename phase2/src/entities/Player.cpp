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
#include "core/SpriteUtils.hpp"

Player::Player()
    : sprite(std::nullopt), leftTexture(nullptr), rightTexture(nullptr), shootTexture(nullptr),
      noseTexture(nullptr), noseSprite(std::nullopt),
      position(0.f, 0.f), velocity(0.f, 0.f), facingRight(true), shooting(false), shrinkScale(1.f) {
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
            if (!shooting) {
                sprite->setTexture(*leftTexture);
            }
        }
    } else if (right && !left) {
        velocity.x = Constants::PLAYER_MOVE_SPEED;
        if (!facingRight) {
            facingRight = true;
            if (!shooting) {
                sprite->setTexture(*rightTexture);
            }
        }
    } else {
        velocity.x = 0.f;
    }

    // Phase 2: keep the shooting-pose sprite mirrored to whichever way the
    // player is currently facing, even while a direction key is held down
    // (spec §3.1 — shooting must stay fully independent of movement).
    if (shooting && shootTexture) {
        applyScale();
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

    if (shooting && noseSprite) {
        float visualHalfHeight = (shootOpaqueBounds.size.y / 2.f) * shrinkScale;
        float headTopY = position.y - visualHalfHeight;
        noseSprite->setPosition({position.x, headTopY});
        noseSprite->setScale({shrinkScale, shrinkScale});
        window.draw(*noseSprite);
    }
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
    shooting = false;
    shrinkScale = 1.f;

    sprite->setTexture(*rightTexture, true);
    sf::FloatRect idleBounds = sprite->getLocalBounds();
    sprite->setOrigin({idleBounds.size.x / 2.f, idleBounds.size.y / 2.f});
    applyScale();
    sprite->setPosition(position);
}

void Player::configureShooting(const sf::Texture& shootPoseTexture) {
    shootTexture = &shootPoseTexture;

    shootOpaqueBounds = SpriteUtils::computeOpaqueBounds(shootPoseTexture);
}

void Player::configureNose(const sf::Texture& noseTex) {
    noseTexture = &noseTex;
    noseOpaqueBounds = SpriteUtils::computeOpaqueBounds(noseTex);
    noseSprite.emplace(*noseTexture);

    noseSprite->setOrigin({noseOpaqueBounds.position.x + noseOpaqueBounds.size.x / 2.f,
                           noseOpaqueBounds.position.y + noseOpaqueBounds.size.y});
}

void Player::setShooting(bool isShooting) {
    if (!sprite || !shootTexture || shooting == isShooting) {
        shooting = isShooting;
        return;
    }
    shooting = isShooting;


    if (shooting) {
        sprite->setTexture(*shootTexture, true);
        sprite->setOrigin({shootOpaqueBounds.position.x + shootOpaqueBounds.size.x / 2.f,
                            shootOpaqueBounds.position.y + shootOpaqueBounds.size.y / 2.f});
    } else {
        const sf::Texture& idleTexture = facingRight ? *rightTexture : *leftTexture;
        sprite->setTexture(idleTexture, true);
        sf::FloatRect idleBounds = sprite->getLocalBounds();
        sprite->setOrigin({idleBounds.size.x / 2.f, idleBounds.size.y / 2.f});
    }
    applyScale();
}

bool Player::isShootingPose() const {
    return shooting;
}

sf::Vector2f Player::getBulletSpawnPosition() const {
    if (!sprite) {
        return position;
    }

    float visualHalfHeight = (shootOpaqueBounds.size.y / 2.f) * shrinkScale;
    float headTopY = position.y - visualHalfHeight;
    float noseVisibleHeight = noseOpaqueBounds.size.y * shrinkScale;
    return sf::Vector2f(position.x, headTopY - noseVisibleHeight);
}

void Player::setShrinkScale(float scale) {
    shrinkScale = scale;
    applyScale();
}

float Player::getShrinkScale() const {
    return shrinkScale;
}

void Player::applyScale() {
    if (!sprite) {
        return;
    }

    float flip = (shooting && facingRight) ? 1.f : (shooting ? -1.f : 1.f);
    sprite->setScale({flip * shrinkScale, shrinkScale});
}
