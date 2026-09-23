#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <cmath>
#include <algorithm>
#include "../components/transform.h"
#include "../components/collider.h"

namespace calculations
{
    inline bool checkAABB(const TransformComponent& tA, const ColliderComponent& cA,
                          const TransformComponent& tB, const ColliderComponent& cB)
    {
        float leftA   = (tA.position.x + cA.offset.x) - (cA.size.x / 2.0f);
        float rightA  = leftA + cA.size.x;
        float topA    = (tA.position.y + cA.offset.y) - (cA.size.y / 2.0f);
        float bottomA = topA + cA.size.y;

        float leftB   = (tB.position.x + cB.offset.x) - (cB.size.x / 2.0f);
        float rightB  = leftB + cB.size.x;
        float topB    = (tB.position.y + cB.offset.y) - (cB.size.y / 2.0f);
        float bottomB = topB + cB.size.y;

        return (rightA > leftB && leftA < rightB && bottomA > topB && topA < bottomB);
    }

    inline void resolveCollision(TransformComponent& dynamicT, const ColliderComponent& dynamicC,
                                 const TransformComponent& staticT, const ColliderComponent& staticC)
    {
        // 1. Calculate relative delta between collider centers
        float centerDynX = dynamicT.position.x + dynamicC.offset.x;
        float centerDynY = dynamicT.position.y + dynamicC.offset.y;
        float centerStatX = staticT.position.x + staticC.offset.x;
        float centerStatY = staticT.position.y + staticC.offset.y;

        float deltaX = centerDynX - centerStatX;
        float deltaY = centerDynY - centerStatY;

        // 2. Minimum non-colliding distance
        float halfWidthDyn   = dynamicC.size.x / 2.0f;
        float halfWidthStat  = staticC.size.x / 2.0f;
        float halfHeightDyn  = dynamicC.size.y / 2.0f;
        float halfHeightStat = staticC.size.y / 2.0f;

        float overlapX = (halfWidthDyn + halfWidthStat) - std::abs(deltaX);
        float overlapY = (halfHeightDyn + halfHeightStat) - std::abs(deltaY);

        // 3. Resolve along the axis with the least penetration depth
        if (overlapX > 0.0f && overlapY > 0.0f)
        {
            if (overlapX < overlapY)
            {
                // Displace along X
                dynamicT.position.x += (deltaX > 0.0f) ? overlapX : -overlapX;
            }
            else
            {
                // Displace along Y
                dynamicT.position.y += (deltaY > 0.0f) ? overlapY : -overlapY;
            }
        }
    }
}

#endif // CALCULATIONS_H