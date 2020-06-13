//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_WORLDSETTINGS_H
#define GAMEPROTOTYPE_WORLDSETTINGS_H

namespace ProgramInformation
{
    /**
     * Holds the settings used to generate the world. These settings are then used to render the world.
     */
    // TODO: Implement the option for the render window to update these values.
    class WorldSettings
    {
        public:

            /**
             * Get the length of the world in world units.
             *
             * @return world length in world units
             */
            static unsigned int getWorldLength();

            /**
             * Get the length of the a grid section in world units.
             *
             * @return grid section length in world units
             */
            static unsigned int getGridSectionLength();

            /**
             * Get the length of the an individual cube in world units.
             *
             * @return individual cube length in world units
             */
            static unsigned int getIndividualCubeLength();

            /**
             * Get the number of surface cubes within a grid section.
             *
             * @return number of surface cubes within a grid section
             */
            static unsigned int getSurfaceCubesPerGridSection();

        private:

            static unsigned int worldLength;
            static unsigned int gridSectionLength;
            static unsigned int individualCubeLength;
    };
}

#endif //GAMEPROTOTYPE_WORLDSETTINGS_H
