//
// Created by BinyBrion on 2019-10-29.
//

#include "DynamicAABB.h"
#include "StaticAABB.h"

namespace World
{
    namespace BoundingVolumes
    {

        DynamicAABB::DynamicAABB(XRange xRange, YRange yRange, ZRange zRange) : AABB{xRange, yRange, zRange}
        {

        }

        bool DynamicAABB::checkIntersectionAABB(const DynamicAABB& otherAABB) const
        {
            return AABB::checkIntersectionAABB(otherAABB);
        }

        bool DynamicAABB::checkIntersectionAABB(const World::BoundingVolumes::StaticAABB &otherAABB) const
        {
            return AABB::checkIntersectionAABB(otherAABB);
        }

        bool DynamicAABB::operator==(const World::BoundingVolumes::DynamicAABB &otherAABB) const
        {
            return AABB::operator==(otherAABB);
        }

        void DynamicAABB::translate(glm::vec3 translation)
        {
            xRange.translate(translation[0]);
            yRange.translate(translation[1]);
            zRange.translate(translation[2]);
        }
    }
}