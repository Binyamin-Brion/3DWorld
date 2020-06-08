//
// Created by BinyBrion on 2019-10-29.
//

#include "AABB.h"

namespace World
{
    namespace BoundingVolumes
    {
        bool AABB::checkIntersectionPoint(glm::vec3 point) const
        {
            return xRange.pointWithin(point[0]) &&
                   yRange.pointWithin(point[1]) &&
                   zRange.pointWithin(point[2]);
        }

        XRange AABB::getXRange() const
        {
            return xRange;
        }

        YRange AABB::getYRange() const
        {
            return yRange;
        }

        ZRange AABB::getZRange() const
        {
            return zRange;
        }

        // Beginning of protected functions

        AABB::AABB(XRange xRange, YRange yRange, ZRange zRange) : xRange{xRange}, yRange{yRange}, zRange{zRange}
        {

        }

        bool AABB::checkIntersectionAABB(const AABB &otherAABB) const
        {
            return xRange.overlapRange(otherAABB.xRange) &&
                   yRange.overlapRange(otherAABB.yRange) &&
                   zRange.overlapRange(otherAABB.zRange);
        }

        bool AABB::operator==(const AABB &otherAABB) const
        {
            return xRange == otherAABB.xRange &&
                   yRange == otherAABB.yRange &&
                   zRange == otherAABB.zRange;
        }
    }
}