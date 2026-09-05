/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/MonsterManager.hpp"
#include "core/Constants.hpp"
#include "core/CollisionUtils.hpp"
#include <algorithm>
#include <limits>

MonsterManager::MonsterManager(ResourceManager<sf::Texture>& textureManager, std::mt19937& rng)
    : textures(textureManager), randomEngine(rng),
      difficulty(Difficulty::Easy),
      highestConsideredY(std::numeric_limits<float>::max()),
      platformsSinceLastSpawn(0) {
}

MonsterManager::~MonsterManager() {
    clear();
}

void MonsterManager::reset(Difficulty newDifficulty) {
    difficulty = newDifficulty;
    highestConsideredY = std::numeric_limits<float>::max();
    platformsSinceLastSpawn = 0;
    clear();
}

void MonsterManager::update(float deltaTime) {
    for (Monster* monster : monsters) {
        monster->update(deltaTime);
    }
}

int MonsterManager::healthForDifficulty() const {
    if (difficulty == Difficulty::Medium) return Constants::MONSTER_HEALTH_MEDIUM;
    if (difficulty == Difficulty::Hard) return Constants::MONSTER_HEALTH_HARD;
    return Constants::MONSTER_HEALTH_EASY;
}

float MonsterManager::spawnChanceForDifficulty() const {
    if (difficulty == Difficulty::Medium) return Constants::MONSTER_SPAWN_CHANCE_MEDIUM;
    if (difficulty == Difficulty::Hard) return Constants::MONSTER_SPAWN_CHANCE_HARD;
    return Constants::MONSTER_SPAWN_CHANCE_EASY;
}

int MonsterManager::minPlatformGapForDifficulty() const {
    if (difficulty == Difficulty::Medium) return Constants::MONSTER_MIN_PLATFORM_GAP_MEDIUM;
    if (difficulty == Difficulty::Hard) return Constants::MONSTER_MIN_PLATFORM_GAP_HARD;
    return Constants::MONSTER_MIN_PLATFORM_GAP_EASY;
}

bool MonsterManager::isEligible(const Platform& platform, float platformY, float currentHeight) const {
    if (platformsSinceLastSpawn <= minPlatformGapForDifficulty()) return false;
    if ((-currentHeight) - platformY < Constants::HAZARD_MIN_LOOKAHEAD_BUFFER) return false;
    if (CollisionUtils::isMovingPlatform(platform)) return false; // never chases a moving platform
    return randomRange(0.f, 1.f) <= spawnChanceForDifficulty();
}

void MonsterManager::maybeSpawnNew(const std::vector<std::unique_ptr<Platform>>& platforms,
                                    std::vector<sf::FloatRect> obstacleBounds,
                                    float currentHeight) {
    bool gateOpen = currentHeight >= Constants::MONSTER_MIN_SPAWN_HEIGHT;
    float newWatermark = highestConsideredY;

    for (const auto& platform : platforms) {
        float platformY = platform->getPosition().y;
        if (platformY >= highestConsideredY) {
            continue;
        }
        // Considered the instant it appears, gate open or not — this is
        // what stops a backlog of monsters dumping in at once once the
        // gate opens (spec feedback: no sudden appearances at the start).
        newWatermark = std::min(newWatermark, platformY);
        ++platformsSinceLastSpawn;

        if (!gateOpen || !isEligible(*platform, platformY, currentHeight)) {
            continue;
        }
        if (CollisionUtils::isWithinSpringReach(platformY, platforms, Constants::SPRING_HAZARD_SAFE_DISTANCE)) {
            continue; // never within a single spring boost's reach (spec feedback)
        }

        const sf::Texture& texture =
            (randomRange(0.f, 1.f) < 0.5f) ? textures.get("monster_blue") : textures.get("monster_green");
        sf::Vector2u texSize = texture.getSize();

        std::vector<sf::FloatRect> combined = obstacleBounds;
        for (Monster* existing : monsters) {
            combined.push_back(existing->getBounds());
        }

        auto placement = CollisionUtils::findSidePlacement(
            platform->getPosition(), platform->getSize(),
            static_cast<float>(texSize.x), static_cast<float>(texSize.y), Constants::HAZARD_SIDE_GAP,
            Constants::MONSTER_PATROL_RANGE, static_cast<float>(Constants::WINDOW_WIDTH), combined, 6.f);

        if (placement.found) {
            float speed = (placement.farBoundX >= placement.nearBoundX) ? Constants::MONSTER_SPEED
                                                                          : -Constants::MONSTER_SPEED;
            monsters.push_back(new Monster(texture, sf::Vector2f(placement.spawnX, placement.centerY),
                                            speed, placement.nearBoundX, placement.farBoundX,
                                            healthForDifficulty()));
            platformsSinceLastSpawn = 0;
        }
    }

    highestConsideredY = newWatermark;
}

void MonsterManager::removeOffscreen(float viewTopY) {
    auto it = std::remove_if(monsters.begin(), monsters.end(),
        [viewTopY](Monster* monster) {
            bool tooLow = monster->getPosition().y >
                          viewTopY + Constants::WINDOW_HEIGHT + Constants::PLATFORM_FALL_MARGIN;
            if (tooLow || monster->isMarkedForRemoval()) {
                delete monster;
                return true;
            }
            return false;
        });
    monsters.erase(it, monsters.end());
}

void MonsterManager::render(sf::RenderWindow& window) const {
    for (const Monster* monster : monsters) {
        monster->render(window);
    }
}

void MonsterManager::clear() {
    for (Monster* monster : monsters) {
        delete monster;
    }
    monsters.clear();
}

MonsterHitResult MonsterManager::checkPlayerCollision(const sf::FloatRect& playerBoundsRaw,
                                                       float playerVelocityY,
                                                       float previousPlayerBottom) {
    // Bug fix (spec feedback: "must EXACTLY touch to lose, not just get
    // close"): both the player and the monster sprites are round/blob
    // shapes that never fill the corners of their own rectangular bounding
    // box, so two full AABBs can register as touching well before the
    // actual drawn art does. Inset both boxes before testing.
    sf::FloatRect playerBounds = CollisionUtils::inset(playerBoundsRaw, Constants::MONSTER_HITBOX_INSET_RATIO);

    float playerBottom = playerBounds.position.y + playerBounds.size.y;
    float playerLeft    = playerBounds.position.x;
    float playerRight   = playerBounds.position.x + playerBounds.size.x;

    for (Monster* monster : monsters) {
        sf::FloatRect monsterBounds = CollisionUtils::inset(monster->getBounds(), Constants::MONSTER_HITBOX_INSET_RATIO);

        if (!playerBounds.findIntersection(monsterBounds).has_value()) {
            continue;
        }

        bool horizontalOverlap =
            playerRight > monsterBounds.position.x &&
            playerLeft  < monsterBounds.position.x + monsterBounds.size.x;

        bool wasAbove = previousPlayerBottom <= monsterBounds.position.y + monsterBounds.size.y * 0.5f;

        bool fallingOntoTop =
            playerVelocityY > 0.f && horizontalOverlap && wasAbove &&
            playerBottom <= monsterBounds.position.y + monsterBounds.size.y * 0.5f + Constants::COLLISION_TOLERANCE;

        return fallingOntoTop ? MonsterHitResult::Bounce : MonsterHitResult::Kill;
    }

    return MonsterHitResult::None;
}

std::vector<sf::Vector2f> MonsterManager::checkBulletCollisions(std::vector<Bullet*>& bullets) {
    std::vector<sf::Vector2f> killPositions;

    for (Bullet* bullet : bullets) {
        if (bullet->isMarkedForRemoval()) {
            continue;
        }
        sf::FloatRect bulletBounds = bullet->getBounds();

        for (Monster* monster : monsters) {
            if (monster->isMarkedForRemoval()) {
                continue;
            }
            if (bulletBounds.findIntersection(monster->getBounds()).has_value()) {
                bool died = monster->takeDamage(1);
                bullet->markForRemoval();
                if (died) {
                    killPositions.push_back(monster->getPosition());
                }
                break;
            }
        }
    }

    removeMarked();
    return killPositions;
}

std::vector<sf::FloatRect> MonsterManager::getAllBounds() const {
    std::vector<sf::FloatRect> bounds;
    bounds.reserve(monsters.size());
    for (const Monster* monster : monsters) {
        bounds.push_back(monster->getBounds());
    }
    return bounds;
}

void MonsterManager::removeMarked() {
    auto it = std::remove_if(monsters.begin(), monsters.end(),
        [](Monster* monster) {
            if (monster->isMarkedForRemoval()) {
                delete monster;
                return true;
            }
            return false;
        });
    monsters.erase(it, monsters.end());
}

float MonsterManager::randomRange(float minValue, float maxValue) const {
    std::uniform_real_distribution<float> distribution(minValue, maxValue);
    return distribution(randomEngine);
}
