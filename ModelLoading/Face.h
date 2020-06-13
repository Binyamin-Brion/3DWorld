//
// Created by binybrion on 4/30/20.
//

#ifndef VOXEL_L_SYSTEM_FACE_H
#define VOXEL_L_SYSTEM_FACE_H

#include <vec3.hpp>
#include "mat4x4.hpp"

namespace ModelLoading
{
    /**
     * Represents a face in the loaded mesh. This face is a triangular face, composed of three points in a 3D world.
     */

    class Face
    {
        public:

            /**
             * Initializes this face with vertices in local space.
             *
             * @param firstPoint first vertex that makes up the triangular face
             * @param secondPoint second vertex that makes up the triangular face
             * @param thirdPoint third vertex that makes up the triangular face
             */
            Face(glm::vec3 firstPoint, glm::vec3 secondPoint, glm::vec3 thirdPoint);

            /**
             * Checks for an intersection with a ray in the world and the face. Since the face represents the face of the mesh
             * in local space, a transformation matrix has to be provided to check for an instance of the face in world space.
             *
             * @param cameraPosition position of the camera when checking for an intersection
             * @param rayDirection direction of the ray that may intersect with the face
             * @param transformationMatrix matrix that transform the face from local space to world space
             * @return true if there is an intersection
             */
            bool checkIntersection(const glm::vec3 &cameraPosition, const glm::vec3 &rayDirection, const glm::mat4x4 &transformationMatrix) const;

        private:

            const glm::vec3 firstPoint;
            const glm::vec3 secondPoint;
            const glm::vec3 thirdPoint;
    };
}

#endif //VOXEL_L_SYSTEM_FACE_H
