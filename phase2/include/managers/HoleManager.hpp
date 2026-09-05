/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef HOLE_MANAGER_HPP
#define HOLE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <memory>
#include "entities/Hole.hpp"
#include "entities/Platform.hpp"
#include "core/ResourceManager.hpp"
#include "core/Difficulty.hpp"


class HoleManager {
public:
    HoleManager(ResourceManager<sf::Texture>& textureManager, std::mt19937& rng);
    ~HoleManager();

    HoleManager(const HoleManager&) = delete;
    HoleManager& operator=(const HoleManager&) = delete;

    void reset(Difficulty difficulty);


    void maybeSpawnNew(const std::vector<std::unique_ptr<Platform>>& platforms,
                        std::vector<sf::FloatRect> obstacleBounds,
                        float currentHeight);

    void removeOffscreen(float viewTopY);
    void render(sf::RenderWindow& window) const;
    void clear();

    // Returns the hole the player is currently overlapping, or nullptr.
    const Hole* checkPlayerCollision(const sf::FloatRect& playerBounds) const;

    std::vector<sf::FloatRect> getAllBounds() const;

private:
    float randomRange(float minValue, float maxValue) const;
    bool isEligible(const Platform& platform, float platformY, float currentHeight) const;

    ResourceManager<sf::Texture>& textures;
    std::mt19937& randomEngine;
    std::vector<Hole*> holes;

    Difficulty difficulty;
    float highestConsideredY;
    int platformsSinceLastSpawn; // enforces Constants::HOLE_MIN_PLATFORM_GAP
};

#endif
