//
// Created by BinyBrion on 2019-10-29.
//

#include "StaticAABB.h"

namespace World
{
    namespace BoundingVolumes
    {
        StaticAABB::StaticAABB(XRange xRange, YRange yRange, ZRange zRange) : AABB{xRange, yRange, zRange}
        {

        }

        bool StaticAABB::checkIntersectionAABB(const StaticAABB &otherAABB) const
        {
            return AABB::checkIntersectionAABB(otherAABB);
        }

        bool StaticAABB::operator==(const StaticAABB &otherAABB) const
        {
            return AABB::operator==(otherAABB);
        }

        StaticAABB createTranslatedStaticAABB(const BoundingVolumes::StaticAABB &staticAabb, glm::vec3 translation)
        {
            return BoundingVolumes::StaticAABB
                    {
                        XRange{staticAabb.getXRange().getMin() + translation.x, staticAabb.getXRange().getMax() + translation.x},
                        YRange{staticAabb.getYRange().getMin() + translation.y, staticAabb.getYRange().getMax() + translation.y},
                        ZRange{staticAabb.getZRange().getMin() + translation.z, staticAabb.getZRange().getMax() + translation.z},
                    };
        }
    }
}