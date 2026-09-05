/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef HOLE_HPP
#define HOLE_HPP

#include <SFML/Graphics.hpp>

class Hole {
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    bool markedForRemoval;

public:
    Hole(const sf::Texture& texture, const sf::Vector2f& startPosition, float scale);

    void render(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getCenter() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

    bool isMarkedForRemoval() const;
    void markForRemoval();
};

#endif
