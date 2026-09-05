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

class PlatformManager {
public:
    PlatformManager(ResourceManager<sf::Texture>& textureManager, std::mt19937& rng);

    void spawnInitial();
    void update(float deltaTime);
    void checkCollisions(Player& player, float previousPlayerBottom);
    void maybeSpawnNew(float viewTopY);
    void removeOffscreen(float viewTopY);
    void render(sf::RenderWindow& window);
    void clear();

private:
    std::unique_ptr<Platform> createRandom(float y);
    float randomRange(float minValue, float maxValue);

    float currentMaxGap() const;

    ResourceManager<sf::Texture>& textures;
    std::mt19937&                 randomEngine;
    std::vector<std::unique_ptr<Platform>> platforms;

    bool lastSpawnWasBreakable;
};

#endif
