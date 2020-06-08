//
// Created by BinyBrion on 2019-10-29.
//

#include "YRange.h"

namespace World
{

    YRange::YRange(float min, float max)
            :
                Range{min, max}
    {

    }

    bool YRange::operator==(const World::YRange &otherRange) const
    {
        return Range::operator==(otherRange);
    }

    bool YRange::overlapRange(YRange yRange) const
    {
        return Range::overlapRange(yRange);
    }
}
