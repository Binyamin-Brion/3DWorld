//
// Created by binybrion on 4/9/20.
//

#ifndef GLFW_TERRAINRANGE_H
#define GLFW_TERRAINRANGE_H

namespace World
{
    namespace WorldLogic
    {
        /**
         * Holds the range of columns or rows of a game world.
         *
         * Note: the range represented is [begin, end).
         */

        struct TerrainRange
        {
            unsigned int begin;
            unsigned int end;

            /**
             * Checks if two terrain ranges are equal- both the begin and variable must match.
             *
             * @param other the TerrainRange to compare
             * @return true if both ranges represent the same section of the game world
             */
            bool operator==(const TerrainRange &other) const;
        };
    }
}

#endif //GLFW_TERRAINRANGE_H
