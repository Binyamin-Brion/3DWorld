//
// Created by BinyBrion on 2019-12-22.
//

#ifndef MINECRAFTOPENGL_HELPERFUNCTIONS_H
#define MINECRAFTOPENGL_HELPERFUNCTIONS_H

#include <vec3.hpp>
#include <vector>

namespace ModelLoading
{
    /**
     *  Combines the minimum point and maximum point in any dimension into one logical unit. Since each member is a
     *  3D vectors, all the required dimensions can be represented. The value stored are in model space.
     */

    struct BoundingBoxExtremes
    {
        glm::vec3 minPoint;
        glm::vec3 maxPoint;
    };

    /**
     * Determines the minimum and maximum point in all three dimensions from a set of vertices. If the input set of
     * vertices is empty, then the origin is given for both minimum and maximum values. Result given is in model space.
     *
     * @param vertices from which to find an enclosing AABB
     * @return points in three dimensions that can be used to create an AABB
     */
    BoundingBoxExtremes findAABB(const std::vector<glm::vec3> &vertices);
}

#endif //MINECRAFTOPENGL_HELPERFUNCTIONS_H
