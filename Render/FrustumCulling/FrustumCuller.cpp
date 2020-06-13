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

        planeCoefficients[Left] = glm::vec4{ matrix[3][0] + matrix[0][0],
                                             matrix[3][1] + matrix[0][1],
                                             matrix[3][2] + matrix [0][2],
                                             matrix[3][3] + matrix[0][3]};

        planeCoefficients[Right] = glm::vec4{ matrix[3][0] - matrix[0][0],
                                             matrix[3][1] - matrix[0][1],
                                             matrix[3][2] - matrix [0][2],
                                             matrix[3][3] - matrix[0][3]};

        planeCoefficients[Bottom] = glm::vec4{ matrix[3][0] + matrix[1][0],
                                             matrix[3][1] + matrix[1][1],
                                             matrix[3][2] + matrix [1][2],
                                             matrix[3][3] + matrix[1][3]};

        planeCoefficients[Top] = glm::vec4{ matrix[3][0] - matrix[1][0],
                                              matrix[3][1] - matrix[1][1],
                                              matrix[3][2] - matrix [1][2],
                                              matrix[3][3] - matrix[1][3]};

        planeCoefficients[Near] = glm::vec4{ matrix[3][0] + matrix[2][0],
                                               matrix[3][1] + matrix[2][1],
                                               matrix[3][2] + matrix [2][2],
                                               matrix[3][3] + matrix[2][3]};

        planeCoefficients[Far] = glm::vec4{ matrix[3][0] - matrix[2][0],
                                            matrix[3][1] - matrix[2][1],
                                            matrix[3][2] - matrix [2][2],
                                            matrix[3][3] - matrix[2][3]};

        for(auto &i : planeCoefficients)
        {
            i = glm::normalize(i);
        }
    }

    bool FrustumCuller::pointInFrustum(const glm::vec3 &point, bool testingGridSection) const
    {
        int currentPlaneIndex = -1;

        for(const auto &i : planeCoefficients)
        {
            currentPlaneIndex += 1;

            // Since the grid sections y-dimensions expand infinitely, they will always fail the top and bottom
            // plane check. Thus they are excluded.
            if(testingGridSection && (currentPlaneIndex == Top || currentPlaneIndex == Bottom))
            {
                continue;
            }

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
}