//
// Created by BinyBrion on 2019-10-29.
//

#include "XRange.h"

namespace World
{
    XRange::XRange(float min, float max)
            :
                Range{min, max}
    {

    }

    bool XRange::operator==(const XRange &otherRange) const
    {
        return Range::operator==(otherRange);
    }

    bool XRange::overlapRange(XRange xRange) const
    {
        return Range::overlapRange(xRange);
    }
}