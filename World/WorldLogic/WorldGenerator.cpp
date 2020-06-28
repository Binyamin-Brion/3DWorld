//
// Created by binybrion on 6/12/20.
//

#include <chrono>
#include "WorldGenerator.h"
#include "../../ProgramInformation/WorldSettings.h"
#include "FastNoise.h"
#include "../../ProgramInformation/AssetLocations.h"
#include "poisson_disk_sampling.h"

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

        fillGaps();

        addTrees();
    }

    const std::vector<std::vector<GridSection>> &WorldGenerator::getTerrainData() const
    {
        return gridSections;
    }

    // Beginning of private functions

    void WorldGenerator::addTrees()
    {
        std::vector<std::vector<Objects::StaticObject>> treeStaticObject;

        treeStaticObject.insert(treeStaticObject.end(), gridSections.size(), std::vector<Objects::StaticObject>{});

        for(auto &i : treeStaticObject)
        {
            i.insert(i.end(), gridSections.size(), Objects::StaticObject{ProgramInformation::AssetLocations::getTreeAssetLocation()});
        }

    //    Objects::StaticObject treeStaticObject{ProgramInformation::AssetLocations::getTreeAssetLocation()};

        float treeRadius = 20.0f;

        std::array<float, 2> minPoint{0, 0};

        std::array<float, 2> maxPoint{500, 500};

        auto treeLocations = thinks::PoissonDiskSampling(treeRadius, minPoint, maxPoint);

        for(const auto &i : treeLocations)
        {
            unsigned int xPos = i[0];
            unsigned int zPos = i[1];

            unsigned int gridSectionIndex_X = xPos * 2 / ProgramInformation::WorldSettings::getGridSectionLength();

            unsigned int gridSectionIndex_Z = zPos * 2/ ProgramInformation::WorldSettings::getGridSectionLength();

            treeStaticObject[gridSectionIndex_X][gridSectionIndex_Z].addPosition(glm::vec3{xPos * 2, heightMap[xPos][zPos] + 1, zPos * 2});
        }

        for(unsigned int i = 0; i < gridSections.size(); ++i)
        {
            for(unsigned int j = 0; j < gridSections[i].size(); ++j)
            {
                gridSections[i][j].addStaticObject(treeStaticObject[i][j]);
            }
        }

//        treeStaticObject.addPosition(glm::vec3{0.f, 75.f, 0.f});
//
//        gridSections[0][0].addStaticObject(treeStaticObject);
//
//        Objects::StaticObject flowerStaticObject{ProgramInformation::AssetLocations::getFlowerAssetLocation()};
//
//        flowerStaticObject.addPosition(glm::vec3{5.f, 75.f, 0.f});
//
//        gridSections[0][0].addStaticObject(flowerStaticObject);
    }

    unsigned char WorldGenerator::getHeightEast(unsigned int xPos, unsigned int zPos) const
    {
        return heightMap[xPos + 1][zPos];
    }

    unsigned char WorldGenerator::getHeightNorth(unsigned int xPos, unsigned int zPos) const
    {
        return heightMap[xPos][zPos - 1];
    }

    unsigned char WorldGenerator::getHeightSouth(unsigned int xPos, unsigned int zPos) const
    {
        return heightMap[xPos][zPos + 1];
    }

    unsigned char WorldGenerator::getHeightWest(unsigned int xPos, unsigned int zPos) const
    {
        return heightMap[xPos - 1][zPos];
    }

    void WorldGenerator::fillGaps()
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        const unsigned int cubeLength = ProgramInformation::WorldSettings::getIndividualCubeLength();

        #pragma omp parallel for default(none)
        for (unsigned int xIndex = 0; xIndex < heightMap.size(); ++xIndex)
        {
            for (unsigned int zIndex = 0; zIndex < heightMap[xIndex].size(); ++zIndex)
            {
                // Get the height of the neighbour cubes.

                unsigned char heightNorth = heightMap[xIndex][zIndex];
                unsigned char heightSouth = heightMap[xIndex][zIndex];
                unsigned char heightEast = heightMap[xIndex][zIndex];
                unsigned char heightWest = heightMap[xIndex][zIndex];

                if (zIndex != 0)
                {
                    heightNorth = getHeightNorth(xIndex, zIndex);
                }

                if (zIndex != (heightMap[xIndex].size() - 1))
                {
                    heightSouth = getHeightSouth(xIndex, zIndex);
                }

                if (xIndex != 0)
                {
                    heightWest = getHeightWest(xIndex, zIndex);
                }

                if (xIndex != (heightMap.size() - 1))
                {
                    heightEast = getHeightEast(xIndex, zIndex);
                }

                // Find the lowest neighbour cube height.

                unsigned char lowestHeight_North_South = std::min(heightNorth, heightSouth);
                unsigned char lowestHeight_West_East = std::min(heightWest, heightEast);
                unsigned char lowestNeighbourHeight = std::min(lowestHeight_North_South, lowestHeight_West_East);

                // This is the height that the current cube's height will be compared against.
                unsigned int gapFillingHeight = lowestNeighbourHeight + ProgramInformation::WorldSettings::getIndividualCubeLength();

                unsigned int currentHeight = heightMap[xIndex][zIndex];

                // Keep adding cubes to fill the gap until there is no longer a gap.
                while (currentHeight > gapFillingHeight)
                {
                    unsigned int gridSectionIndex_X = xIndex * cubeLength / ProgramInformation::WorldSettings::getGridSectionLength();
                    unsigned int gridSectionIndex_Z = zIndex * cubeLength / ProgramInformation::WorldSettings::getGridSectionLength();

                    BoundingVolumes::StaticAABB gapFillingCubeAABB
                            {
                                    XRange(xIndex * cubeLength, xIndex * cubeLength + cubeLength),
                                    YRange(currentHeight - cubeLength, currentHeight),
                                    ZRange(zIndex * cubeLength, zIndex * cubeLength + cubeLength)
                            };

                    // More than one thread might access the same grid section!
                    #pragma omp critical
                    {
                        gridSections[gridSectionIndex_X][gridSectionIndex_Z].addSurfaceCube(gapFillingCubeAABB);
                    }

                    currentHeight -= ProgramInformation::WorldSettings::getIndividualCubeLength();
                }
            }
        }
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

                unsigned int xOffset = gridSection.getSurroundingCube().getXRange().getMin();
                unsigned int zOffset = gridSection.getSurroundingCube().getZRange().getMin();

                BoundingVolumes::StaticAABB surfaceCube
                        {
                                XRange(xOffset + x, xOffset + x + cubeLength),
                                YRange(cubeHeight, cubeHeight + cubeLength),
                                ZRange(zOffset + z, zOffset + z + cubeLength),
                        };

                gridSection.addSurfaceCube(surfaceCube);

                heightMap[heightMapX][heightMapZ] = cubeHeight;
            }
        }
    }
}
