/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/PlatformManager.hpp"
#include "entities/NormalPlatform.hpp"
#include "entities/MovingPlatform.hpp"
#include "entities/BreakablePlatform.hpp"
#include "core/Constants.hpp"
#include <algorithm>
#include <array>

PlatformManager::PlatformManager(ResourceManager<sf::Texture>& textureManager, std::mt19937& rng)
    : textures(textureManager), randomEngine(rng), lastSpawnWasBreakable(false) {
}

float PlatformManager::currentMaxGap() const {
    return lastSpawnWasBreakable
        ? Constants::MAX_GAP_AFTER_BREAKABLE
        : Constants::MAX_VERTICAL_GAP;
}

std::unique_ptr<Platform> PlatformManager::createRandom(float y) {
    float x = randomRange(0.f, static_cast<float>(Constants::WINDOW_WIDTH) - Constants::PLATFORM_WIDTH);
    sf::Vector2f pos(x, y);

    double breakableWeight = lastSpawnWasBreakable ? 0.0 : Constants::BREAKABLE_PLATFORM_WEIGHT;

    std::array<double, 3> weights = {
        Constants::NORMAL_PLATFORM_WEIGHT,
        Constants::MOVING_PLATFORM_WEIGHT,
        breakableWeight
    };
    std::discrete_distribution<int> typeDistribution(weights.begin(), weights.end());
    int choice = typeDistribution(randomEngine);

    if (choice == 2) {
        lastSpawnWasBreakable = true;
        return std::make_unique<BreakablePlatform>(textures.get("broken_platform"), pos);
    }

    lastSpawnWasBreakable = false;

    if (choice == 1) {
        return std::make_unique<MovingPlatform>(textures.get("moving_platform"), pos,
                                                Constants::MOVING_PLATFORM_SPEED,
                                                static_cast<float>(Constants::WINDOW_WIDTH));
    }

    std::unique_ptr<Platform> platform =
        std::make_unique<NormalPlatform>(textures.get("normal_platform"), pos);

    NormalPlatform* normalPlatform = dynamic_cast<NormalPlatform*>(platform.get());
    if (normalPlatform != nullptr &&
        randomRange(0.f, 1.f) < Constants::SPRING_SPAWN_CHANCE) {
        normalPlatform->attachSpring(std::make_unique<Spring>(textures.get("spring")));
    }

    return platform;
}

void PlatformManager::spawnInitial() {
    clear();
    lastSpawnWasBreakable = false;

    platforms.push_back(std::make_unique<NormalPlatform>(
        textures.get("normal_platform"),
        sf::Vector2f(Constants::WINDOW_WIDTH  / 2.f - Constants::PLATFORM_WIDTH / 2.f,
                     Constants::WINDOW_HEIGHT / 2.f + Constants::PLAYER_INITIAL_SPAWN_OFFSET)));

    float y = Constants::WINDOW_HEIGHT / 2.f;
    while (y > -Constants::MIN_VERTICAL_GAP) {

        float maxGap = currentMaxGap();
        y -= randomRange(Constants::MIN_VERTICAL_GAP, maxGap);
        platforms.push_back(createRandom(y));
    }
}

void PlatformManager::update(float deltaTime) {
    for (auto& platform : platforms) {
        platform->update(deltaTime);
    }
}

void PlatformManager::checkCollisions(Player& player, float previousPlayerBottom) {
    if (player.getVelocityY() <= 0.f) {
        return;
    }

    sf::FloatRect playerBounds = player.getBounds();
    float playerBottom = playerBounds.position.y + playerBounds.size.y;
    float playerLeft   = playerBounds.position.x;
    float playerRight  = playerBounds.position.x + playerBounds.size.x;

    for (auto& platform : platforms) {
        if (!platform->canJumpOn()) {
            continue;
        }

        sf::FloatRect platBounds = platform->getBounds();

        bool horizontalOverlap =
            playerRight > platBounds.position.x &&
            playerLeft  < platBounds.position.x + platBounds.size.x;

        bool wasAbove =
            previousPlayerBottom <= platBounds.position.y + Constants::COLLISION_SINK_TOLERANCE;

        bool nowReachesTop =
            playerBottom >= platBounds.position.y &&
            playerBottom <= platBounds.position.y + platBounds.size.y + Constants::COLLISION_TOLERANCE;

        if (horizontalOverlap && wasAbove && nowReachesTop) {
            if (platform->hasSpring() &&
                platform->getSpring()->getBounds().findIntersection(playerBounds).has_value()) {
                platform->getSpring()->trigger();
                player.applyVerticalBoost(platform->getSpring()->getBoostVelocity());
            } else {
                float boost = platform->onPlayerLand();
                if (boost != 0.f) {
                    player.applyVerticalBoost(boost);
                }
            }
            break;
        }
    }
}

void PlatformManager::maybeSpawnNew(float viewTopY) {
    float highestY = viewTopY + Constants::WINDOW_HEIGHT;
    for (auto& platform : platforms) {
        if (platform->getPosition().y < highestY) {
            highestY = platform->getPosition().y;
        }
    }

    while (highestY > viewTopY - Constants::MIN_VERTICAL_GAP) {
        float maxGap = currentMaxGap();
        highestY -= randomRange(Constants::MIN_VERTICAL_GAP, maxGap);
        platforms.push_back(createRandom(highestY));
    }
}

void PlatformManager::removeOffscreen(float viewTopY) {
    platforms.erase(
        std::remove_if(platforms.begin(), platforms.end(),
            [viewTopY](const std::unique_ptr<Platform>& platform) {
                bool tooLow = platform->getPosition().y >
                              viewTopY + Constants::WINDOW_HEIGHT + Constants::PLATFORM_FALL_MARGIN;
                return tooLow || platform->isMarkedForRemoval();
            }),
        platforms.end());
}

void PlatformManager::render(sf::RenderWindow& window) {
    for (auto& platform : platforms) {
        platform->render(window);
    }
}

void PlatformManager::clear() {
    platforms.clear();
}

float PlatformManager::randomRange(float minValue, float maxValue) {
    std::uniform_real_distribution<float> distribution(minValue, maxValue);
    return distribution(randomEngine);
}
