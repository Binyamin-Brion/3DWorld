//
// Created by BinyBrion on 2019-10-29.
//

#ifndef MINECRAFTQT_AABB_H
#define MINECRAFTQT_AABB_H

#include "../RangeImplementation/XRange.h"
#include "../RangeImplementation/YRange.h"
#include "../RangeImplementation/ZRange.h"

#include <vec3.hpp>

namespace World
{
    namespace BoundingVolumes
    {
        /*
         * This is the base implementation of all AABB.
         *
         * It bundles together Ranges of the X, Y, and Z dimension
         * to provide a logical AABB. It passes arguments to its
         * respective Ranges to determine if a intersection has occurred.
         *
         * This class is NOT intended to be used in a polymorphic fashion.
         * It has no virtual destructor, and some functions marked protected
         * are intended to be redefined in inherited classes. Redefined functions
         * call the implementation defined here.
         */

        class AABB
        {
            public:
                bool checkIntersectionPoint(glm::vec3 point) const;
                XRange getXRange() const;
                YRange getYRange() const;
                ZRange getZRange() const;

            protected:
                AABB(XRange xRange, YRange yRange, ZRange zRange);

                // This function is protected as it should be redefined
                // in inherited class to ensure that only correct types of AABB
                // are intersection against one another. See DynamicAABB for an example.
                bool checkIntersectionAABB(const AABB& otherAABB) const;
                bool operator==(const AABB &otherAABB) const;

                XRange xRange;
                YRange yRange;
                ZRange zRange;
        };
    }
}

#endif //MINECRAFTQT_AABB_H
