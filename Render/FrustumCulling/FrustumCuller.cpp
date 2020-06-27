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

        for(const auto &i : planeCoefficients)
        {
            currentPlaneIndex += 1;

            float distanceToPlane = i.x * point.x +
                                    i.y * point.y +
                                    i.z * point.z +
                                    i.w;

            if(distanceToPlane < 0)
            {
                return false;
            }
        }

        return true;
    }

    bool FrustumCuller::pointInFrustumNoHeight(glm::vec3 point, const glm::vec3 &cameraPos, const glm::vec3 &cameraFront) const
    {
        // Find what the height of the point should be given its location from the camera. Doing so ensures that the y-component
        // is in view of the frustum, making it effectively redundant (which is required when calling this function).
        float distanceToCamera = glm::length(glm::vec2{point.x - cameraPos.x, point.z - cameraPos.z});

        point.y = (cameraPos + cameraFront * distanceToCamera).y ;

        return pointInFrustum(point);
    }
}