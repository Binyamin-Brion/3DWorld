//
// Created by binybrion on 4/14/20.
//

#ifndef GLFW_FRUSTUMCULLER_H
#define GLFW_FRUSTUMCULLER_H

#include <mat4x4.hpp>
#include <vec4.hpp>

namespace Render::FrustumCulling
{
    /**
     * Provides the ability to determine if a point in the game world is visible or not.
     */

    class FrustumCuller
    {
        public:

            /**
             * Determines the equations for the planes that make up a frustum. This should be called every time
             * the camera's view or projection matrix changes.
             *
             * @param matrix the combined projection * view matrix
             */
            void updatePlaneCoefficients(glm::mat4 matrix);

            /**
             * Determines if a point is in the camera's frustum, and is thus visible.
             *
             * @param point to check for visibility
             * @return true if the point is visible
             */
            [[nodiscard]] bool pointInFrustum(const glm::vec3 &point) const;

            /**
             * Determines if a point is in the camera's frustum based off of the x-z plane; ie the height of the point
             * is not considered.
             *
             * @param point to check for visibility
             * @param cameraPos position of the camera
             * @param cameraFront the direction the camera is looking at
             * @return true if the point is visible
             */
            [[nodiscard]] bool pointInFrustumNoHeight(glm::vec3 point, const glm::vec3 &cameraPos, const glm::vec3 &cameraFront) const;

        private:

            // Holds the equations of the frustum planes
            glm::vec4 planeCoefficients[6];

    };
}

#endif //GLFW_FRUSTUMCULLER_H
