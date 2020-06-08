//
// Created by BinyBrion on 2019-10-29.
//

#ifndef MINECRAFTQT_XRANGE_H
#define MINECRAFTQT_XRANGE_H

#include "Range.h"

namespace World
{
    /*
     * Specialized range for the X dimension in the world.
     */

    class XRange : public Range
    {
        public:
          XRange(float min, float max);
          bool operator==(const XRange &otherRange) const;
          bool overlapRange(XRange xRange) const;
    };
}

#endif //MINECRAFTQT_XRANGE_H
