//
// Created by BinyBrion on 2019-10-29.
//

#include "ZRange.h"

namespace World
{

    ZRange::ZRange(float min, float max)
            :
                Range{min, max}
    {

    }

    bool ZRange::operator==(const World::ZRange &otherRange) const
    {
        return Range::operator==(otherRange);
    }

    bool ZRange::overlapRange(ZRange zRange) const
    {
        return Range::overlapRange(zRange);
    }
}
