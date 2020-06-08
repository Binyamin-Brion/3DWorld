//
// Created by BinyBrion on 2019-10-29.
//

#ifndef MINECRAFTQT_YRANGE_H
#define MINECRAFTQT_YRANGE_H

#include "Range.h"

namespace World
{
    /*
     * Specialized range for the Y dimension for the world.
     */

    class YRange : public Range
    {
        public:
            YRange(float min, float max);
            bool operator==(const YRange &otherRange) const;
            bool overlapRange(YRange yRange) const;
    };
}

#endif //MINECRAFTQT_YRANGE_H
