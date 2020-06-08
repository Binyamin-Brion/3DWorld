//
// Created by binybrion on 4/27/20.
//

#ifndef GLFW_NEWLYADDEDCUBES_H
#define GLFW_NEWLYADDEDCUBES_H

#include "../BoundingVolumes/StaticAABB.h"
#include <vector>

namespace World
{
    namespace WorldLogic
    {
        /**
         * Holds cubes that were added to the world when loading new sections of the world.
         */

        struct NewlyAddedCubes
        {
            BoundingVolumes::StaticAABB surroundingAABB;
            std::vector<BoundingVolumes::StaticAABB> newCubes;
        };
    }
}

#endif //GLFW_NEWLYADDEDCUBES_H
