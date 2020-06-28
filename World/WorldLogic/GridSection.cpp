//
// Created by binybrion on 4/9/20.
//

#include "GridSection.h"

namespace World::WorldLogic
{
    GridSection::GridSection(BoundingVolumes::StaticAABB surroundingAABB, unsigned int gridSectionID)
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

        gridID = gridSectionID;
    }

    void GridSection::addSurfaceCube(BoundingVolumes::StaticAABB cubeAABB)
    {
        surfaceCubes.push_back(cubeAABB);
    }

    void GridSection::addStaticObject(Objects::StaticObject staticObject)
    {
        staticObjects.push_back(std::move(staticObject));
    }

    unsigned int GridSection::getGridSectionID() const
    {
        return gridID;
    }

    const std::vector<BoundingVolumes::StaticAABB>& GridSection::getSurfaceCubes() const
    {
        return surfaceCubes;
    }

    const BoundingVolumes::StaticAABB &GridSection::getSurroundingCube() const
    {
        return surroundingAABB;
    }

    const std::vector<Objects::StaticObject> &GridSection::getStaticObjects() const
    {
        return staticObjects;
    }
}
