//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_WORLDGENERATOR_H
#define GAMEPROTOTYPE_WORLDGENERATOR_H

#include "GridSection.h"

namespace World::WorldLogic
{
    /**
     * Generates a world in which the game takes place.
     */

    class WorldGenerator
    {
        public:

            /**
             * Creates a world using the world settings specified in the ProgramInformation::WorldSettings class.
             */
            void createWorld();

            /**
             * Get the information about the world created so that it can rendered and logic applied to the world.
             *
             * @return most-recent generated world data
             */
            [[nodiscard]] const std::vector<std::vector<GridSection>>& getTerrainData() const;

        private:

            /**
             * Fills the passed in grid section with surface cubes.
             *
             * @param gridSection in which to place surface cubes
             */
            void fillGridSectionSurfaceCubes(GridSection &gridSection);

            // Every index into grid sections represents a columns, and the vector of that column represents rows in the world.
            std::vector<std::vector<GridSection>> gridSections;
    };
}

#endif //GAMEPROTOTYPE_WORLDGENERATOR_H
