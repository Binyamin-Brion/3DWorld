//
// Created by binybrion on 4/12/20.
//

#include "TerrainRange.h"

namespace World
{
    namespace WorldLogic
    {
        bool TerrainRange::operator==(const World::WorldLogic::TerrainRange &other) const
        {
            return begin == other.begin && end == other.end;
        }
    }
}