//
// Created by BinyBrion on 2019-10-29.
//

#ifndef MINECRAFTQT_ZRANGE_H
#define MINECRAFTQT_ZRANGE_H

#include "Range.h"

namespace World
{
    /*
     * Specialization of Range for the Z dimension of the world.
     */

    class ZRange : public Range
    {
        public:
            ZRange(float min, float max);
            bool operator==(const ZRange &otherRange) const;
            bool overlapRange(ZRange zRange) const;
    };
}

#endif //MINECRAFTQT_ZRANGE_H
