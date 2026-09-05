/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "managers/BulletManager.hpp"
#include <algorithm>

BulletManager::~BulletManager() {
    clear();
}

void BulletManager::spawn(const sf::Vector2f& spawnPosition) {
    bullets.push_back(new Bullet(spawnPosition));
}

void BulletManager::update(float deltaTime, float viewTopY) {
    for (Bullet* bullet : bullets) {
        bullet->update(deltaTime);
        if (bullet->isOffScreen(viewTopY)) {
            bullet->markForRemoval();
        }
    }
    removeMarked();
}

void BulletManager::render(sf::RenderWindow& window) const {
    for (const Bullet* bullet : bullets) {
        bullet->render(window);
    }
}

void BulletManager::removeMarked() {
    auto it = std::remove_if(bullets.begin(), bullets.end(),
        [](Bullet* bullet) {
            if (bullet->isMarkedForRemoval()) {
                delete bullet;
                return true;
            }
            return false;
        });
    bullets.erase(it, bullets.end());
}

void BulletManager::clear() {
    for (Bullet* bullet : bullets) {
        delete bullet;
    }
    bullets.clear();
}

std::vector<Bullet*>& BulletManager::getBullets() {
    return bullets;
}
