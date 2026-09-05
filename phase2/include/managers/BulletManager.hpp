/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef BULLET_MANAGER_HPP
#define BULLET_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "entities/Bullet.hpp"

class BulletManager {
public:
    BulletManager() = default;
    ~BulletManager();

    BulletManager(const BulletManager&) = delete;
    BulletManager& operator=(const BulletManager&) = delete;

    void spawn(const sf::Vector2f& spawnPosition);
    void update(float deltaTime, float viewTopY);
    void render(sf::RenderWindow& window) const;
    void clear();

    std::vector<Bullet*>& getBullets();

private:
    void removeMarked();

    std::vector<Bullet*> bullets;
};

#endif
