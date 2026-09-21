#ifndef CALCULATIONS_H
#define CALCULATIONS_H

namespace calculations
{
    template<typename T1, typename T2>
    inline bool checkAABB(const T1& tA, const T2& cA, const T1& tB, const T2& cB)
    {
        float leftA   = (tA.x + cA.offset.x) - (cA.size.x / 2.0f);
        float rightA  = leftA + cA.size.x;
        float topA    = (tA.y + cA.offset.y) - (cA.size.y / 2.0f);
        float bottomA = topA + cA.size.y;

        float leftB   = (tB.x + cB.offset.x) - (cB.size.x / 2.0f);
        float rightB  = leftB + cB.size.x;
        float topB    = (tB.y + cB.offset.y) - (cB.size.y / 2.0f);
        float bottomB = topB + cB.size.y;

        return (rightA > leftB && leftA < rightB && bottomA > topB && topA < bottomB);
    }

    template<typename T>
    inline void resolveCollision(T& dynamicT, const T& staticT)
    {
        
    }
}
#endif