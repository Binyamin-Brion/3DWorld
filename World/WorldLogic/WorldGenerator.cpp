//
// Created by binybrion on 6/12/20.
//

#include <chrono>
#include "WorldGenerator.h"
#include "../../ProgramInformation/WorldSettings.h"

namespace World::WorldLogic
{
    void WorldGenerator::createWorld()
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        // Simply aliases for actual value for readability purposes.

        const unsigned int gridSectionLength = ProgramInformation::WorldSettings::getGridSectionLength();

        const unsigned int yRangeLimits = std::numeric_limits<unsigned int>::max();

        gridSections.clear();

        unsigned int numberColumns = ProgramInformation::WorldSettings::getWorldLength() / ProgramInformation::WorldSettings::getGridSectionLength();

        // Insert the required amount of columns before the loop to prevent need for modifying the vector in a parellel
        // loop, which could lead to data races.
        gridSections.insert(gridSections.begin(), numberColumns, std::vector<GridSection>{});

        // Create grid sections and fill them with surface cubes.

        #pragma omp parallel for default(shared)
        for(unsigned int x = 0; x < gridSections.size(); ++x)
        {
            // Have a local copy of the "base" grid section ID for the current column for each thread to avoid having
            // a critical section.
            unsigned int gridSectionID = x * numberColumns;

            for(unsigned int z = 0; z < gridSections.size(); ++z)
            {
                unsigned int xOffset = x * ProgramInformation::WorldSettings::getGridSectionLength();
                unsigned int zOffset = z * ProgramInformation::WorldSettings::getGridSectionLength();

                BoundingVolumes::StaticAABB gridSectionAABB
                        {
                            XRange(xOffset, xOffset + gridSectionLength),
                            YRange(-yRangeLimits, yRangeLimits),
                            ZRange(zOffset, zOffset + gridSectionLength)
                        };

                gridSections[x].emplace_back(gridSectionAABB, gridSectionID);

                gridSectionID += 1;

                fillGridSectionSurfaceCubes(gridSections[x][z]);
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
