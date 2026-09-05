/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#include "core/SpriteUtils.hpp"
#include <algorithm>

namespace SpriteUtils {

sf::FloatRect computeOpaqueBounds(const sf::Texture& texture) {
    sf::Image image = texture.copyToImage();
    sf::Vector2u size = image.getSize();

    unsigned int minX = size.x;
    unsigned int minY = size.y;
    unsigned int maxX = 0;
    unsigned int maxY = 0;
    bool foundAny = false;

    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            if (image.getPixel({x, y}).a > 0) {
                foundAny = true;
                minX = std::min(minX, x);
                minY = std::min(minY, y);
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);
            }
        }
    }

    if (!foundAny) {

        return sf::FloatRect({0.f, 0.f},
                              {static_cast<float>(size.x), static_cast<float>(size.y)});
    }

    return sf::FloatRect({static_cast<float>(minX), static_cast<float>(minY)},
                          {static_cast<float>(maxX - minX + 1), static_cast<float>(maxY - minY + 1)});
}

}
