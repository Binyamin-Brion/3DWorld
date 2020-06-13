//
// Created by binybrion on 6/12/20.
//

#include "WorldSettings.h"

namespace ProgramInformation
{
    unsigned int WorldSettings::worldLength = 6400;
    unsigned int WorldSettings::gridSectionLength = 32;
    unsigned int WorldSettings::individualCubeLength = 2;

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

    unsigned int WorldSettings::getSurfaceCubesPerGridSection()
    {
        return (gridSectionLength / individualCubeLength) * (gridSectionLength / individualCubeLength);
    }
}