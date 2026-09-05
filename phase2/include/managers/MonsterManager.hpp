/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef MONSTER_MANAGER_HPP
#define MONSTER_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <memory>
#include "entities/Monster.hpp"
#include "entities/Platform.hpp"
#include "entities/Bullet.hpp"
#include "core/ResourceManager.hpp"
#include "core/Difficulty.hpp"

enum class MonsterHitResult {
    None,
    Bounce, 
    Kill    
};


class MonsterManager {
public:
    MonsterManager(ResourceManager<sf::Texture>& textureManager, std::mt19937& rng);
    ~MonsterManager();

    MonsterManager(const MonsterManager&) = delete;
    MonsterManager& operator=(const MonsterManager&) = delete;

    void reset(Difficulty difficulty);
    void update(float deltaTime);


    void maybeSpawnNew(const std::vector<std::unique_ptr<Platform>>& platforms,
                        std::vector<sf::FloatRect> obstacleBounds,
                        float currentHeight);

    void removeOffscreen(float viewTopY);
    void render(sf::RenderWindow& window) const;
    void clear();

    MonsterHitResult checkPlayerCollision(const sf::FloatRect& playerBounds,
                                           float playerVelocityY,
                                           float previousPlayerBottom);

    std::vector<sf::Vector2f> checkBulletCollisions(std::vector<Bullet*>& bullets);

    std::vector<sf::FloatRect> getAllBounds() const;

private:
    void removeMarked();
    float randomRange(float minValue, float maxValue) const;

    int healthForDifficulty() const;
    float spawnChanceForDifficulty() const;
    int minPlatformGapForDifficulty() const;
    bool isEligible(const Platform& platform, float platformY, float currentHeight) const;

    ResourceManager<sf::Texture>& textures;
    std::mt19937& randomEngine;
    std::vector<Monster*> monsters;

    Difficulty difficulty;
    float highestConsideredY;
    int platformsSinceLastSpawn; 
};

#endif
