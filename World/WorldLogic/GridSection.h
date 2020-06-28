//
// Created by binybrion on 4/9/20.
//

#ifndef GLFW_GRIDSECTION_H
#define GLFW_GRIDSECTION_H

#include <vector>
#include "../BoundingVolumes/StaticAABB.h"
#include "../../Objects/StaticObject.h"

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
             * @param gridSectionID the ID if this grid section
             */
            GridSection(BoundingVolumes::StaticAABB surroundingAABB, unsigned int gridSectionID);

            /**
             * Add a surface cube (ie that is visible) to the grid section.
             *
             * @param cubeAABB of the visible surface cube
             */
            void addSurfaceCube(BoundingVolumes::StaticAABB cubeAABB);

            /**
             * Add an additional static object to this grid section.
             *
             * @param staticObject
             */
            void addStaticObject(Objects::StaticObject staticObject);

            /**
             * Get the ID of this grid section.
             *
             * @return grid section's ID
             */
            [[nodiscard]] unsigned int getGridSectionID() const;

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
             * Get the static objects stored in this grid section.
             *
             * @return vector of stored static objects
             */
            [[nodiscard]] const std::vector<Objects::StaticObject>& getStaticObjects() const;

        private:
            BoundingVolumes::StaticAABB surroundingAABB;
            std::vector<BoundingVolumes::StaticAABB> surfaceCubes;
            std::vector<Objects::StaticObject> staticObjects;

            unsigned int gridID;
    };
}

#endif //GLFW_GRIDSECTION_H
