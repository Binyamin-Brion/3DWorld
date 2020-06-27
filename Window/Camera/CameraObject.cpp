//
// Created by BinyBrion on 8/11/2019.
//

#include <gtc/matrix_transform.hpp>
#include "CameraObject.h"
#include <cstdio>

namespace Window::Camera
{
    CameraObject::CameraObject()
                    :
                        maxAngleLookUp{89.0f},
                        maxAngleLookDown{-30.0f},
                        position{750.f, 95.f, 750.f},
                        front{0.f, 0.f, -1.0f},
                        up{0.0f, 1.0f, 0.0f},
                        yaw{-90.0f},
                        pitch{0.0f},
                        mouseSensitivity{0.1f},
                        movementSpeed{0.5f},
                        firstRotation{true},
                        drawDistance{384.0f},
                        view{glm::lookAt(position, position + front, up)}

    {

    }

    float CameraObject::getDrawDistance() const
    {
        return drawDistance;
    }

    const glm::vec3 &CameraObject::getFront() const
    {
        return front;
    }

    float CameraObject::getMovementSpeed() const
    {
        return movementSpeed;
    }

    const glm::mat4 &CameraObject::getProjectionMatrix() const
    {
        return projection;
    }

    const glm::mat4 &CameraObject::getViewMatrix() const
    {
        return view;
    }

    glm::vec3 CameraObject::getPosition() const
    {
        return position;
    }

    glm::vec3 CameraObject::getWorldCoordinates(int screenWidth, int screenHeight, int mouseX, int mouseY) const
    {
        glm::vec4 viewPort{0, screenHeight, screenWidth, -screenHeight};

        glm::vec2 mouseCoords{mouseX, mouseY};

        glm::mat4 matrix = glm::mat4(1.0f);

        auto result = glm::unProject(glm::vec3{mouseX, mouseY, 0.0f}, matrix, projection * view, viewPort);

        return result;
    }

    void CameraObject::move(CameraMovement cameraMovement)
    {
        if (CameraMovement::Backwards == cameraMovement)
        {
            position -= front * movementSpeed;
        }
        else if (CameraMovement::Forwards == cameraMovement)
        {
            position += front * movementSpeed;
        }
        else if (CameraMovement::Left == cameraMovement)
        {
            position -= glm::normalize(glm::cross(front, up)) * movementSpeed;
        }
        else if (CameraMovement::Right == cameraMovement)
        {
            position += glm::normalize(glm::cross(front, up)) * movementSpeed;
        }
printf("Pos: %f, %f, %f \n", position.x, position.y, position.z);
        view = glm::lookAt(position, position + front, up);
    }

    void CameraObject::resetFirstRotation()
    {
        firstRotation = true;
    }

    void CameraObject::rotate(int mouseX, int mouseY)
    {
        if (firstRotation)
        {
            lastRotationX = mouseX;

            lastRotationY = mouseY;

            firstRotation = false;
        }

        float xMouseOffset = mouseX - lastRotationX;
        float yMouseOffset = lastRotationY - mouseY;

        lastRotationX = mouseX;
        lastRotationY = mouseY;

        xMouseOffset *= mouseSensitivity;
        yMouseOffset *= mouseSensitivity;

        yaw += xMouseOffset;
        pitch += yMouseOffset;

        // Prevent issues that occur when the pitch is 90 degrees or -90 degrees.
        if (pitch > maxAngleLookUp)
        {
            pitch = maxAngleLookUp;
        }

        if (pitch < maxAngleLookDown)
        {
            pitch = maxAngleLookDown;
        }

        glm::vec3 newCameraFront;

        newCameraFront.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        newCameraFront.y = sinf(glm::radians(pitch));
        newCameraFront.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

        front = glm::normalize(newCameraFront);

        view = glm::lookAt(position, position + front, up);

    }

    void CameraObject::setCameraPosition(glm::vec3 cameraPosition)
    {
        position = cameraPosition;

        view = glm::lookAt(position, position + front, up);
    }

    void CameraObject::updateDrawDistance(float drawDistance)
    {
        this->drawDistance = drawDistance;

        projection = glm::perspective(glm::radians(45.0f), screenDimensionsRatio, 0.1f, drawDistance);
    }

    void CameraObject::updateScreenSize(int screenWidth, int screenHeight)
    {
        screenDimensionsRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

        projection = glm::perspective(glm::radians(45.0f), screenDimensionsRatio, 0.1f, drawDistance);
    }
}