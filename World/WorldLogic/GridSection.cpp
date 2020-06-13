//
// Created by binybrion on 4/9/20.
//

#include "GridSection.h"

namespace World::WorldLogic
{
    unsigned int GridSection::globalGridID = 0;

    GridSection::GridSection(BoundingVolumes::StaticAABB surroundingAABB)
                    :
                        surroundingAABB{surroundingAABB}
    {
        assert(surroundingAABB.getXRange().getMin() >= 0);
        assert(surroundingAABB.getXRange().getMax() >= 0);

        assert(surroundingAABB.getYRange().getMin() >= 0);
        assert(surroundingAABB.getYRange().getMax() >= 0);

        assert(surroundingAABB.getZRange().getMin() >= 0);
        assert(surroundingAABB.getZRange().getMax() >= 0);

        // Explicit casts to prevent compiler warnings about conversions known to be safe.
        unsigned int xRange = static_cast<unsigned int>(surroundingAABB.getXRange().getMax() - surroundingAABB.getXRange().getMin());
        unsigned int zRange = static_cast<unsigned int>(surroundingAABB.getZRange().getMax() - surroundingAABB.getZRange().getMin());

        for(unsigned int x = 0; x < xRange; ++x)
        {
            heightMap.emplace_back();

            for(unsigned int z = 0; z < zRange; ++z)
            {
                unsigned int initialCubeHeight = 0;

                heightMap.back().push_back(initialCubeHeight);
            }
        }

        gridID = globalGridID;

        globalGridID += 1;
    }

    void GridSection::addSurfaceCube(BoundingVolumes::StaticAABB cubeAABB)
    {
        surfaceCubes.push_back(cubeAABB);
    }

    unsigned int GridSection::getGridSectionID() const
    {
        return gridID;
    }

    unsigned int GridSection::getHeightAt(unsigned int x, unsigned int z) const
    {
        return heightMap[x][z];
    }

    const std::vector<BoundingVolumes::StaticAABB>& GridSection::getSurfaceCubes() const
    {
        return surfaceCubes;
    }

    const BoundingVolumes::StaticAABB &GridSection::getSurroundingCube() const
    {
        return surroundingAABB;
    }

    void GridSection::setHeight(unsigned int x, unsigned int z, unsigned height)
    {
        heightMap[x][z] = height;
    }
}
