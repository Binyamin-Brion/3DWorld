//
// Created by binybrion on 4/30/20.
//

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/intersect.hpp>
#include "Face.h"

namespace ModelLoading
{
    Face::Face(glm::vec3 firstPoint, glm::vec3 secondPoint, glm::vec3 thirdPoint)
            :
                firstPoint{firstPoint},
                secondPoint{secondPoint},
                thirdPoint{thirdPoint}
    {

    }

    bool Face::checkIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection, const glm::mat4x4 &transformationMatrix) const
    {
        // These two variables are required in order to call the glm function that does the intersection test. The result
        // stored in these variables are not required to know if there was an intersection.
        glm::vec2 _unused_baryCentricResult;
        float _unused;

        // The intersection test is done in world space, thus appropriate world space vertices need to be calculated.
        glm::vec3 transformedFirst = glm::vec3{ transformationMatrix * glm::vec4{firstPoint, 1.0f}};
        glm::vec3 transformedSecond = glm::vec3{ transformationMatrix * glm::vec4{secondPoint, 1.0f}};
        glm::vec3 transformedThird = glm::vec3{ transformationMatrix * glm::vec4{thirdPoint, 1.0f}};

        return glm::intersectRayTriangle(cameraPosition, rayDirection, transformedFirst, transformedSecond, transformedThird, _unused_baryCentricResult, _unused);
    }
}