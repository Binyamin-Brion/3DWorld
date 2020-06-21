//
// Created by binybrion on 4/14/20.
//

#include "FrustumCuller.h"
#include "FrustumPlane.h"

namespace Render::FrustumCulling
{
    void FrustumCuller::updatePlaneCoefficients(glm::mat4 matrix)
    {
        matrix = glm::transpose(matrix);

        planeCoefficients[Left] = matrix[3] + matrix[0];
        planeCoefficients[Right] = matrix[3] - matrix[0];

        planeCoefficients[Bottom] = matrix[3] + matrix[1];
        planeCoefficients[Top] = matrix[3] - matrix[1];

        planeCoefficients[Near] = matrix[3] + matrix[2];
        planeCoefficients[Far] = matrix[3] - matrix[2];

        for(auto &i : planeCoefficients)
        {
            i = glm::normalize(i);
        }
    }

    bool FrustumCuller::pointInFrustum(const glm::vec3 &point) const
    {
        int currentPlaneIndex = -1;

        bool intersectionResult = false;

        for(const auto &i : planeCoefficients)
        {
            currentPlaneIndex += 1;

            float distanceToPlane = i.x * point.x +
                                    i.y * point.y +
                                    i.z * point.z +
                                    i.w;

            intersectionResult |= (distanceToPlane > 0);
        }

        return intersectionResult;
    }
}