/*
* ========== Naming Convention Guideline ==========
* Class names    : PascalCase
* Function names : camelCase
* Variable names : camelCase
* Constant names : UPPER_SNAKE_CASE
* =================================================
*/

#ifndef COLLISION_UTILS_HPP
#define COLLISION_UTILS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include "entities/Platform.hpp"
#include "entities/MovingPlatform.hpp"

// Shared by every "where can I spawn this new object" decision (platforms,
// monsters, holes). Instead of each manager writing its own overlap test,
// they all funnel their candidate rectangle through this single function.
namespace CollisionUtils {

// Returns true if `candidate` (optionally inflated by `padding` on every
// side) intersects any rectangle in `obstacles`.
inline bool overlapsAny(const sf::FloatRect& candidate,
                         const std::vector<sf::FloatRect>& obstacles,
                         float padding = 0.f) {
    sf::FloatRect inflated(
        {candidate.position.x - padding, candidate.position.y - padding},
        {candidate.size.x + 2.f * padding, candidate.size.y + 2.f * padding});

    for (const sf::FloatRect& obstacle : obstacles) {
        if (inflated.findIntersection(obstacle).has_value()) {
            return true;
        }
    }
    return false;
}

// Bug fix (spec feedback — spring launches you straight into an invisible
// monster): returns true if any platform with an attached spring sits
// below `candidateY` (i.e. spring's Y > candidateY) within `safeDistance`
// world-units — meaning a single spring boost could reach a hazard placed
// at candidateY before the player ever gets a chance to see it.
inline bool isWithinSpringReach(float candidateY,
                                 const std::vector<std::unique_ptr<Platform>>& platforms,
                                 float safeDistance) {
    for (const auto& platform : platforms) {
        if (!platform->hasSpring()) {
            continue;
        }
        float springY = platform->getPosition().y;
        float verticalGap = springY - candidateY; // positive: spring sits below the candidate
        if (verticalGap >= 0.f && verticalGap <= safeDistance) {
            return true;
        }
    }
    return false;
}

// Bug fix (spec feedback — "must EXACTLY touch, not just get close"):
// shrinks a rectangle by `insetRatio` on every side so collision checks
// operate on roughly the visible round silhouette instead of its full
// rectangular bounding box, whose corners are always empty for round art.
inline sf::FloatRect inset(const sf::FloatRect& rect, float insetRatio) {
    float insetX = rect.size.x * insetRatio;
    float insetY = rect.size.y * insetRatio;
    return sf::FloatRect({rect.position.x + insetX, rect.position.y + insetY},
                          {rect.size.x - 2.f * insetX, rect.size.y - 2.f * insetY});
}

// Bug fix (spec feedback — "platform and monster must never overlap or
// pass through each other"): a MovingPlatform keeps sliding sideways for
// as long as it exists, so a one-time spawn-time overlap check isn't
// enough — it could drift into a hazard placed beside it later. Hazards
// are simply never placed beside a MovingPlatform at all (dynamic_cast,
// per spec §8.3), which is a hard guarantee instead of a probabilistic one.
inline bool isMovingPlatform(const Platform& platform) {
    return dynamic_cast<const MovingPlatform*>(&platform) != nullptr;
}

// Result of trying to place a hazard beside a platform, on whichever side
// (left/right) had room. `found == false` means neither side worked.
struct SidePlacement {
    bool found = false;
    float spawnX = 0.f;
    float centerY = 0.f;
    float nearBoundX = 0.f; // == spawnX; the edge closest to the platform
    float farBoundX = 0.f;  // spawnX pushed `patrolRange` further away (0 => static item)
};

// Shared by MonsterManager and HoleManager: picks whichever side of
// `platform` (right first, then left) can fit an item of the given size
// without overlapping anything in `obstacles`.
inline SidePlacement findSidePlacement(const sf::Vector2f& platformPos, const sf::Vector2f& platformSize,
                                        float itemWidth, float itemHeight, float gap, float patrolRange,
                                        float screenWidth, const std::vector<sf::FloatRect>& obstacles,
                                        float padding) {
    float centerY = platformPos.y + platformSize.y / 2.f;
    float rightX = platformPos.x + platformSize.x + gap + itemWidth / 2.f;
    float leftX  = platformPos.x - gap - itemWidth / 2.f;

    struct Side { float spawnX; float farX; };
    Side sides[2] = {
        {rightX, std::min(rightX + patrolRange, screenWidth - itemWidth / 2.f)},
        {leftX,  std::max(leftX - patrolRange, itemWidth / 2.f)}
    };

    for (const Side& side : sides) {
        if (side.spawnX - itemWidth / 2.f < 0.f || side.spawnX + itemWidth / 2.f > screenWidth) {
            continue;
        }
        sf::FloatRect candidate({side.spawnX - itemWidth / 2.f, centerY - itemHeight / 2.f},
                                 {itemWidth, itemHeight});
        if (!overlapsAny(candidate, obstacles, padding)) {
            return SidePlacement{true, side.spawnX, centerY, side.spawnX, side.farX};
        }
    }
    return SidePlacement{};
}

}

#endif
