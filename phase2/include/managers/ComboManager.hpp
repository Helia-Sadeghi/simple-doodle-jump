/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef COMBO_MANAGER_HPP
#define COMBO_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "core/ResourceManager.hpp"

class ComboManager {
public:
    explicit ComboManager(ResourceManager<sf::Font>& fontManager);


    float registerKill(const sf::Vector2f& worldPosition);

    void update(float deltaTime);


    void render(sf::RenderWindow& window) const;

    void reset();

    int getComboCount() const;

private:
    struct Popup {
        sf::Text text;
        float age;

        Popup(const sf::Font& font, const std::string& content, unsigned int fontSize,
              const sf::Vector2f& position);
    };

    ResourceManager<sf::Font>& fonts;
    std::vector<Popup> popups;

    int comboCount;
    float timeSinceLastKill;
};

#endif
