//
// Created by binybrion on 6/12/20.
//

#include "WorldGenerator.h"
#include "../../ProgramInformation/WorldSettings.h"

namespace World::WorldLogic
{
    void WorldGenerator::createWorld()
    {
        const unsigned int gridSectionLength = ProgramInformation::WorldSettings::getGridSectionLength();

        const unsigned int yRangeLimits = std::numeric_limits<unsigned int>::max();

        for(unsigned int x = 0; x < ProgramInformation::WorldSettings::getWorldLength(); x += gridSectionLength)
        {
            gridSections.emplace_back();

            for(unsigned int z = 0; z < ProgramInformation::WorldSettings::getWorldLength(); z += gridSectionLength)
            {
                BoundingVolumes::StaticAABB gridSectionAABB
                        {
                            XRange(x, x + gridSectionLength),
                            YRange(-yRangeLimits, yRangeLimits),
                            ZRange(z, z + gridSectionLength)
                        };

                gridSections.back().emplace_back(gridSectionAABB);

                fillGridSectionSurfaceCubes(gridSections.back().back());
            }
        }
    }

    const std::vector<std::vector<GridSection>> &WorldGenerator::getTerrainData() const
    {
        return gridSections;
    }

    void WorldGenerator::fillGridSectionSurfaceCubes(GridSection &gridSection)
    {
        const unsigned int cubeLength = ProgramInformation::WorldSettings::getIndividualCubeLength();

        for(unsigned int x = 0; x < ProgramInformation::WorldSettings::getGridSectionLength(); x += cubeLength)
        {
            for(unsigned int z = 0; z < ProgramInformation::WorldSettings::getGridSectionLength(); z += cubeLength)
            {
                unsigned int cubeHeight = 0;

                BoundingVolumes::StaticAABB surfaceCube
                        {
                            XRange(x, x + cubeLength),
                            YRange(cubeHeight, cubeHeight + cubeLength),
                            ZRange(z, z + cubeLength),
                        };

                gridSection.addSurfaceCube(surfaceCube);
            }
        }
    }
}
