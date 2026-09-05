/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "entities/Spring.hpp"
#include "core/Constants.hpp"

Spring::Spring(const sf::Texture& texture)
    : sprite(texture), compressedTimeRemaining(0.f) {
    sf::Vector2u textureSize = texture.getSize();

    int frameHeight = static_cast<int>(textureSize.y) / Constants::SPRING_FRAMES_COUNT;


    int relaxedHeight = frameHeight - Constants::SPRING_RELAXED_HEIGHT_TRIM;

    relaxedFrame    = sf::IntRect({0, 0},           {static_cast<int>(textureSize.x), relaxedHeight});
    compressedFrame = sf::IntRect({0, frameHeight}, {static_cast<int>(textureSize.x), frameHeight});

    sprite.setTextureRect(relaxedFrame);
    refreshOrigin();
}

void Spring::refreshOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y});
}

void Spring::updatePosition(const sf::Vector2f& platformPosition, const sf::Vector2f& platformSize) {
    float centerX = platformPosition.x + platformSize.x / 2.f;
    float topY    = platformPosition.y;
    sprite.setPosition({centerX, topY});
}

void Spring::update(float deltaTime) {
    if (compressedTimeRemaining > 0.f) {
        compressedTimeRemaining -= deltaTime;
        if (compressedTimeRemaining <= 0.f) {
            compressedTimeRemaining = 0.f;
            sprite.setTextureRect(relaxedFrame);
            refreshOrigin();
        }
    }
}

void Spring::trigger() {
    sprite.setTextureRect(compressedFrame);
    refreshOrigin();
    compressedTimeRemaining = Constants::SPRING_COMPRESS_DURATION;
}

void Spring::render(sf::RenderWindow& window) const {
    window.draw(sprite);
}

sf::FloatRect Spring::getBounds() const {
    return sprite.getGlobalBounds();
}

float Spring::getBoostVelocity() const {
    return Constants::SPRING_JUMP_VELOCITY;
}
