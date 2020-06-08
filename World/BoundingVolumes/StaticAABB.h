//
// Created by BinyBrion on 2019-10-29.
//

#ifndef MINECRAFTQT_STATICAABB_H
#define MINECRAFTQT_STATICAABB_H

#include "AABB.h"

namespace World
{
    namespace BoundingVolumes
    {
        /*
         * A specialized AABB that cannot be moved once placed.
         *
         * Since this AABB specialization is static, it cannot intersect
         * with dynamic objects, but dynamic objects can intersect with this.
         */

        class StaticAABB : public AABB
        {
            public:
                StaticAABB(XRange xRange, YRange yRange, ZRange zRange);
                bool checkIntersectionAABB(const StaticAABB &otherAABB) const;
                bool operator==(const StaticAABB& otherAABB) const;
        };

        StaticAABB createTranslatedStaticAABB(const BoundingVolumes::StaticAABB &staticAabb, glm::vec3 translation);
    }
}

#endif //MINECRAFTQT_STATICAABB_H
