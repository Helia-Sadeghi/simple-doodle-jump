/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/HoleManager.hpp"
#include "core/Constants.hpp"
#include "core/CollisionUtils.hpp"
#include <algorithm>
#include <limits>

HoleManager::HoleManager(ResourceManager<sf::Texture>& textureManager, std::mt19937& rng)
    : textures(textureManager), randomEngine(rng),
      difficulty(Difficulty::Easy),
      highestConsideredY(std::numeric_limits<float>::max()),
      platformsSinceLastSpawn(0) {
}

HoleManager::~HoleManager() {
    clear();
}

void HoleManager::reset(Difficulty newDifficulty) {
    difficulty = newDifficulty;
    highestConsideredY = std::numeric_limits<float>::max();
    platformsSinceLastSpawn = 0;
    clear();
}

bool HoleManager::isEligible(const Platform& platform, float platformY, float currentHeight) const {
    if (platformsSinceLastSpawn <= Constants::HOLE_MIN_PLATFORM_GAP) return false;
    if ((-currentHeight) - platformY < Constants::HAZARD_MIN_LOOKAHEAD_BUFFER) return false;
    if (CollisionUtils::isMovingPlatform(platform)) return false; // never chases a moving platform
    return randomRange(0.f, 1.f) <= Constants::HOLE_SPAWN_CHANCE;
}

void HoleManager::maybeSpawnNew(const std::vector<std::unique_ptr<Platform>>& platforms,
                                 std::vector<sf::FloatRect> obstacleBounds,
                                 float currentHeight) {
    // Holes only exist on Hard difficulty (spec §6).
    if (difficulty != Difficulty::Hard) {
        return;
    }

    bool gateOpen = currentHeight >= Constants::HOLE_MIN_SPAWN_HEIGHT;
    float newWatermark = highestConsideredY;

    for (const auto& platform : platforms) {
        float platformY = platform->getPosition().y;
        if (platformY >= highestConsideredY) {
            continue;
        }
        newWatermark = std::min(newWatermark, platformY);
        ++platformsSinceLastSpawn;

        if (!gateOpen || !isEligible(*platform, platformY, currentHeight)) {
            continue;
        }
        if (CollisionUtils::isWithinSpringReach(platformY, platforms, Constants::SPRING_HAZARD_SAFE_DISTANCE)) {
            continue;
        }

        const sf::Texture& texture = textures.get("hole");
        float scale = (randomRange(0.f, 1.f) < 0.5f) ? Constants::HOLE_SMALL_SCALE : Constants::HOLE_LARGE_SCALE;
        sf::Vector2u texSize = texture.getSize();

        std::vector<sf::FloatRect> combined = obstacleBounds;
        for (Hole* existing : holes) {
            combined.push_back(existing->getBounds());
        }

        auto placement = CollisionUtils::findSidePlacement(
            platform->getPosition(), platform->getSize(),
            static_cast<float>(texSize.x) * scale, static_cast<float>(texSize.y) * scale, Constants::HAZARD_SIDE_GAP,
            0.f, static_cast<float>(Constants::WINDOW_WIDTH), combined, 6.f);

        if (placement.found) {
            holes.push_back(new Hole(texture, sf::Vector2f(placement.spawnX, placement.centerY), scale));
            platformsSinceLastSpawn = 0;
        }
    }

    highestConsideredY = newWatermark;
}

void HoleManager::removeOffscreen(float viewTopY) {
    auto it = std::remove_if(holes.begin(), holes.end(),
        [viewTopY](Hole* hole) {
            bool tooLow = hole->getPosition().y >
                          viewTopY + Constants::WINDOW_HEIGHT + Constants::PLATFORM_FALL_MARGIN;
            if (tooLow || hole->isMarkedForRemoval()) {
                delete hole;
                return true;
            }
            return false;
        });
    holes.erase(it, holes.end());
}

void HoleManager::render(sf::RenderWindow& window) const {
    for (const Hole* hole : holes) {
        hole->render(window);
    }
}

void HoleManager::clear() {
    for (Hole* hole : holes) {
        delete hole;
    }
    holes.clear();
}

const Hole* HoleManager::checkPlayerCollision(const sf::FloatRect& playerBounds) const {
    if (difficulty != Difficulty::Hard) {
        return nullptr;
    }
    for (const Hole* hole : holes) {
        if (playerBounds.findIntersection(hole->getBounds()).has_value()) {
            return hole;
        }
    }
    return nullptr;
}

std::vector<sf::FloatRect> HoleManager::getAllBounds() const {
    std::vector<sf::FloatRect> bounds;
    bounds.reserve(holes.size());
    for (const Hole* hole : holes) {
        bounds.push_back(hole->getBounds());
    }
    return bounds;
}

float HoleManager::randomRange(float minValue, float maxValue) const {
    std::uniform_real_distribution<float> distribution(minValue, maxValue);
    return distribution(randomEngine);
}
