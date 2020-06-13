//
// Created by BinyBrion on 8/11/2019.
//

#ifndef GAMEEDITOR_CAMERA_H
#define GAMEEDITOR_CAMERA_H

#include <mat4x4.hpp>
#include "CameraMovement.h"

namespace Window::Camera
{
    /**
     *  Represents the camera from which the world is viewed.
     *
     *  Note that the camera is only read to be used AFTER the updateScreenSize function is called.
     *  This should be done after the initial screen is created with dimensions. Any subsequent changes to the
     *  dimensions of the screen should also result in a call to the updateScreenSize function.
     */

    class CameraObject
    {
        public:

            /**
             * Initializes the camera to default position and orientation, as well as sets the camera movement
             * and draw distance.
             *
             * Note that the projection matrix is created and updated by calling updateScreenSize()!
             */
            CameraObject();

            /**
             * Get the draw distance of the camera.
             *
             * @return draw used by the camera.
             */
            [[nodiscard]] float getDrawDistance() const;

            /**
             * Get the camera's front vector.
             *
             * @return vector representing the direction the camera is facing.
             */
            [[nodiscard]] const glm::vec3 &getFront() const;

            /**
             * Get the movement speed of the camera.
             *
             * @return the movement speed defined for the camera.
             */
            [[nodiscard]] float getMovementSpeed() const;

            /**
             * Get the projection matrix.
             *
             * @return projection matrix created by the camera
             */
            [[nodiscard]] const glm::mat4 &getProjectionMatrix() const;

            /**
             * Get the view matrix.
             *
             * Note that the projection matrix is created and updated by calling updateScreenSize()!
             *
             * @return view matrix created by the camera
             */
            [[nodiscard]] const glm::mat4 &getViewMatrix() const;

            /**
             * Get the position of the camera.
             *
             * @return position of the camera represented by a vector
             */
            [[nodiscard]] glm::vec3 getPosition() const;

            /**
             * Convert mouse coordinates to world coordinates.
             *
             * @param screenWidth width of the render window
             * @param screenHeight height of the render window
             * @param mouseX x-position of the mouse cursor
             * @param mouseY y-position of the mouse cursor
             * @return vector representing the game-world coordinates of the cursor coordinates
             */
            [[nodiscard]] glm::vec3 getWorldCoordinates(int screenWidth, int screenHeight, int mouseX, int mouseY) const;

            /**
             * Move the camera in the given direction.
             *
             * @param cameraMovement the camera should move in
             */
            void move(CameraMovement cameraMovement);

            /**
             *  Resets the firstRotation variable.
             *
             * If the cursor is moved while its movement is not being tracked, then the next time it is tracked there will
             * likely be a jump in the camera's rotation due to a mismatch between where the program thinks the cursor is
             * and where it actually is. Call this function when the cursor has stopped being tracked.
             */
            void resetFirstRotation();

            /**
             * Rotates the camera based off of the cursor's position.
             *
             * @param mouseX x-position of the cursor
             * @param mouseY y-position of the cursor
             */
            void rotate(int mouseX, int mouseY);

            /**
             * Sets the position of the camera to the input argument.
             *
             * @param cameraPosition position the camera should have
             */
            void setCameraPosition(glm::vec3 cameraPosition);

            /**
             * Change the draw distance of the camera.
             *
             * @param drawDistance new draw distance the camera should have
             */
            void updateDrawDistance(float drawDistance);

            /**
             * Let the camera know of a change in the render window dimensions.
             *
             * Note that the projection matrix is created and updated by calling this function!
             *
             * @param screenWidth of the render window
             * @param screenHeight of the render window
             */
            void updateScreenSize(int screenWidth, int screenHeight);

        private:

            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;

            glm::mat4 view;
            glm::mat4 projection;

            float yaw;
            float pitch;
            float movementSpeed;
            float mouseSensitivity;

            bool firstRotation;
            float lastRotationX;
            float lastRotationY;

            const float DEFAULT_DRAW_DISTANCE = 384.0f;

            float drawDistance;

            // When updating the draw distance, creating a new projection matrix requires the ratio of screen width to height.
            // However this is done in a different function (updateScreenSize) so a member variable is needed to keep track of it,
            float screenDimensionsRatio;
    };
}

#endif //GAMEEDITOR_CAMERA_H
