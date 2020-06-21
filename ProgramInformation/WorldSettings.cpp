//
// Created by binybrion on 6/12/20.
//

#include "WorldSettings.h"

namespace ProgramInformation
{
    unsigned int WorldSettings::worldLength = 3200;
    unsigned int WorldSettings::gridSectionLength = 16;
    unsigned int WorldSettings::individualCubeLength = 2;
    unsigned int WorldSettings::maxHeight = 64;

    unsigned int WorldSettings::getWorldLength()
    {
        return worldLength;
    }

    unsigned int WorldSettings::getGridSectionLength()
    {
        return gridSectionLength;
    }

    unsigned int WorldSettings::getIndividualCubeLength()
    {
        return individualCubeLength;
    }

    unsigned int WorldSettings::getMaxHeight()
    {
        return maxHeight;
    }

    unsigned int WorldSettings::getSurfaceCubesPerGridSection()
    {
        return (gridSectionLength / individualCubeLength) * (gridSectionLength / individualCubeLength);
    }
}