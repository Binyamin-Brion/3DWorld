//
// Created by BinyBrion on 2019-12-22.
//

#include "HelperFunctions.h"

namespace ModelLoading
{
    BoundingBoxExtremes findAABB(const std::vector<glm::vec3> &vertices)
    {
        // Iterate through the vertices, and keep track of which point is the minimum and maximum in all three dimensions.
        // These minimum and maximum numbers are returned by this function.

        glm::vec3 minPoint{0.f, 0.f, 0.f};
        glm::vec3 maxPoint{0.f, 0.f, 0.f};

        for(const auto &vertex : vertices)
        {
            if (vertex.x < minPoint.x)
            {
                minPoint.x = vertex.x;
            }
            else if (vertex.x > maxPoint.x)
            {
                maxPoint.x = vertex.x;
            }

            if (vertex.y < minPoint.y)
            {
                minPoint.y = vertex.y;
            }
            else if (vertex.y > maxPoint.y)
            {
                maxPoint.y = vertex.y;
            }

            if (vertex.z < minPoint.z)
            {
                minPoint.z = vertex.z;
            }
            else if (vertex.z > maxPoint.z)
            {
                maxPoint.z = vertex.z;
            }
        }

        return {minPoint, maxPoint};
    }
}