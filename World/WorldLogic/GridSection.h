//
// Created by binybrion on 4/9/20.
//

#ifndef GLFW_GRIDSECTION_H
#define GLFW_GRIDSECTION_H

#include <vector>
#include "../BoundingVolumes/StaticAABB.h"

namespace World::WorldLogic
{
    /**
     * Represents a part of the loaded game world. Holds all of the entities within the part of the world.
     */

    class GridSection
    {
        public:

            /**
             * Initializes the section of the world with a boundary.
             *
             * @param surroundingAABB boundary for the grid section
             */
            explicit GridSection(BoundingVolumes::StaticAABB surroundingAABB);

            /**
             * Add a surface cube (ie that is visible) to the grid section.
             *
             * @param cubeAABB of the visible surface cube
             */
            void addSurfaceCube(BoundingVolumes::StaticAABB cubeAABB);

            /**
             * Get the ID of this grid section.
             *
             * @return grid section's ID
             */
            [[nodiscard]] unsigned int getGridSectionID() const;

            /**
             * Gets the height of the surface cube at the passed in point.
             *
             * @param x index of the surface cube relative to this grid section; range [0, bounding volumes X-Range max)
             * @param z index of the surface cube relative to this grid section; range [0, bounding volumes Z-Range max)
             * @return height of the surface cube at the given location
             */
            [[nodiscard]] unsigned int getHeightAt(unsigned int x, unsigned int z) const;

            /**
             * Get the surface cubes held within this part of the game world.
             *
             * @return vector of surface cubes AABBs
             */
            [[nodiscard]] const std::vector<BoundingVolumes::StaticAABB>& getSurfaceCubes() const;

            /**
             * Get the surrounding cube that represents the boundary for this grid section.
             *
             * @return AABB of this grid section
             */
            [[nodiscard]] const BoundingVolumes::StaticAABB& getSurroundingCube() const;

            /**
             * Sets the height of the surface cube at the passed in point.
             *
             * @param x index of the surface cube relative to this grid section; range [0, bounding volumes X-Range max)
             * @param z index of the surface cube relative to this grid section; range [0, bounding volumes Z-Range max)
             * @param height height of the surface cube at the given location
             */
            void setHeight(unsigned int x, unsigned int z, unsigned height);

        private:
            BoundingVolumes::StaticAABB surroundingAABB;
            std::vector<BoundingVolumes::StaticAABB> surfaceCubes;
            std::vector<std::vector<unsigned int>> heightMap;

            unsigned int gridID;
            static unsigned globalGridID;
    };
}

#endif //GLFW_GRIDSECTION_H
