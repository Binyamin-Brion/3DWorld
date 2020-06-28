//
// Created by BinyBrion on 2019-10-29.
//

#include <cassert>
#include "Range.h"

namespace World
{
    float Range::getMin() const
    {
        return min;
    }

    float Range::getMax() const
    {
        return max;
    }

    bool Range::pointWithin(float point) const
    {
        return min <= point && point <= max;
    }

    void Range::translate(float amount)
    {
        min += amount;

        max += amount;
    }

    Range::Range(float min, float max)
            :
                min{min},
                max{max}
    {
        assert(max >= min);

        if(max < min)
        {
            assert(false);
        }
    }

    bool Range::operator==(const World::Range &otherRange) const
    {
        return min == otherRange.min && max == otherRange.max;
    }

    bool Range::overlapRange(const Range &otherRange) const
    {
        return min <= otherRange.max && max >= otherRange.min;
    }
}