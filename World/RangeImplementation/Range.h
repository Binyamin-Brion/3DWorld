//
// Created by BinyBrion on 2019-10-29.
//

#ifndef MINECRAFTQT_RANGE_H
#define MINECRAFTQT_RANGE_H

namespace World
{
    /*
     * The base implementation for ranges within the world.
     *
     * It represents a range directly within the world without a translation.
     * For example, represents the range in world coordinates [-1, -3] directly
     * instead of specifying a range of length of 2 and then translating it to negative two.
     *
     * This class is not intended to be used in a polymorphic fashion.
     * It has no virtual destructor, and some functions marked protected
     * are intended to be redefined in inherited classes. Redefined functions
     * call the implementation defined here.
     */

    class Range
    {
        public:

            float getMin() const;
            float getMax() const;
            bool pointWithin(float point) const;
            void translate(float amount);

        protected:

            Range(float min, float max);
            bool operator==(const Range &otherRange) const;
            bool overlapRange(const Range &otherRange) const;

        private:

            float min;
            float max;

    };
}

#endif //MINECRAFTQT_RANGE_H
