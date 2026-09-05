/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef PLATFORM_MANAGER_HPP
#define PLATFORM_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "entities/Platform.hpp"
#include "entities/Player.hpp"
#include "core/ResourceManager.hpp"

class SoundManager; // Phase 2: forward declaration only, no header coupling

class PlatformManager {
public:
    PlatformManager(ResourceManager<sf::Texture>& textureManager, std::mt19937& rng);

    void spawnInitial();
    void update(float deltaTime);
    // `soundManager` is optional (defaults to nullptr) so Phase 1 behavior
    // is unchanged if it isn't passed. When given, plays the jump SFX only
    // for a successful normal/moving-platform landing (spec §4.2) — not
    // for springs or breakables, which stay silent.
    void checkCollisions(Player& player, float previousPlayerBottom, SoundManager* soundManager = nullptr);
    void maybeSpawnNew(float viewTopY);
    void removeOffscreen(float viewTopY);
    void render(sf::RenderWindow& window);
    void clear();

    // Phase 2: read-only access so MonsterManager / HoleManager can spawn
    // their objects beside newly-created platforms (spec §2.1 / §6.1) and
    // feed the shared overlap check (spec §7.1).
    const std::vector<std::unique_ptr<Platform>>& getPlatforms() const;
    std::vector<sf::FloatRect> getAllBounds() const;

    // Phase 2: lets Game scale MovingPlatform speed per difficulty
    // (spec §5.2) without altering the Phase 1 spawn logic itself.
    void setSpeedMultiplier(float multiplier);

private:
    std::unique_ptr<Platform> createRandom(float y);
    float randomRange(float minValue, float maxValue);

    float currentMaxGap() const;

    ResourceManager<sf::Texture>& textures;
    std::mt19937&                 randomEngine;
    std::vector<std::unique_ptr<Platform>> platforms;

    bool lastSpawnWasBreakable;
    float speedMultiplier = 1.f;
};

#endif
