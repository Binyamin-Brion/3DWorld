//
// Created by BinyBrion on 2019-10-29.
//

#ifndef MINECRAFTQT_DYNAMICAABB_H
#define MINECRAFTQT_DYNAMICAABB_H

#include "AABB.h"

namespace Tests
{
    namespace World
    {
        class TestDynamicAABB;
    }
}

namespace World
{
    namespace BoundingVolumes
    {
        class StaticAABB;

        /*
         *  A specialized AABB that can moved around once placed.
         *
         *  The fact that it can be moved around gives it the property
         *  of being able to intersect with static objects, but not the
         *  other way around.
         *
         *  It can intersect with other dynamic objects.
         */

        class DynamicAABB : public AABB
        {
            // For testing purposes
            friend class Tests::World::TestDynamicAABB;

            public:
                DynamicAABB(XRange xRange, YRange yRange, ZRange zRange);
                bool checkIntersectionAABB(const DynamicAABB &otherAABB) const;
                bool checkIntersectionAABB(const StaticAABB &otherAABB) const;
                bool operator==(const DynamicAABB& otherAABB) const;
                void translate(glm::vec3 translation);
        };
    }
}

#endif //MINECRAFTQT_DYNAMICAABB_H
