/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef SPRITE_UTILS_HPP
#define SPRITE_UTILS_HPP

#include <SFML/Graphics.hpp>


namespace SpriteUtils {

sf::FloatRect computeOpaqueBounds(const sf::Texture& texture);

}

#endif
