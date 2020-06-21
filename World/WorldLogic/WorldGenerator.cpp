//
// Created by binybrion on 6/12/20.
//

#include <chrono>
#include "WorldGenerator.h"
#include "../../ProgramInformation/WorldSettings.h"
#include "FastNoise.h"

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

        // Insert the required amount of columns before the loop to prevent need for modifying the vector in a parallel.
        // loop, which could lead to data races.
        gridSections.insert(gridSections.begin(), numberColumns, std::vector<GridSection>{});

        // Each grid contains rows / columns of cubes. This has to be taken into account when creating the heightmap, which works on rows / columns
        // of cubes, not grid sections.
        unsigned int numberSurfaceCubeColumns = ProgramInformation::WorldSettings::getWorldLength() / ProgramInformation::WorldSettings::getIndividualCubeLength();

        // Reserve all of the memory up-front so that it can be used in a multi-thread loop.
        heightMap.insert(heightMap.end(), numberSurfaceCubeColumns, std::vector<unsigned char>{});

        for(auto &i : heightMap)
        {
            i.resize(numberSurfaceCubeColumns);
        }

        FastNoise perlinNoise;
        perlinNoise.SetNoiseType(FastNoise::Perlin);
        perlinNoise.SetFrequency(0.015f);

        // Create grid sections and fill them with surface cubes.

        #pragma omp parallel for default(none) shared(numberColumns, perlinNoise)
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

                fillGridSectionSurfaceCubes(gridSections[x][z], perlinNoise);
            }
        }
    }

    const std::vector<std::vector<GridSection>> &WorldGenerator::getTerrainData() const
    {
        return gridSections;
    }

    void WorldGenerator::fillGridSectionSurfaceCubes(GridSection &gridSection, FastNoise &perlinNoise)
    {
        // Simply aliases for actual value for readability purposes.

        const unsigned int cubeLength = ProgramInformation::WorldSettings::getIndividualCubeLength();

        const unsigned int maxHeight = ProgramInformation::WorldSettings::getMaxHeight();

        for(unsigned int x = 0; x < ProgramInformation::WorldSettings::getGridSectionLength(); x += cubeLength)
        {
            for(unsigned int z = 0; z < ProgramInformation::WorldSettings::getGridSectionLength(); z += cubeLength)
            {
                unsigned int heightMapX = (gridSection.getSurroundingCube().getXRange().getMin() + x) / cubeLength;
                unsigned int heightMapZ = (gridSection.getSurroundingCube().getZRange().getMin() + z) / cubeLength;

                // Make sure that the cube height is 0 or higher!
                unsigned int cubeHeight = perlinNoise.GetNoise(heightMapX, heightMapZ) * maxHeight + maxHeight;

                // This ensures that each cube's height is a factor of the cube height, which is required as the size of an
                // individual cube is not one. By doing this, no cube is ony "half" seen due to the height of its neighbours.
                if(cubeHeight % cubeLength != 0)
                {
                    cubeHeight -= 1;
                }

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
