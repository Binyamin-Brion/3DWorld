//
// Created by binybrion on 6/8/20.
//

#include "RenderWindow.h"


#include <stdexcept>
#include "../ProgramSettings.h"
#include "glad.h"
#include "GLFW/glfw3.h"

namespace Window
{
    void GLAPIENTRY MessageCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar* message,
                                    const void* userParam);

    Camera::CameraObject RenderWindow::camera;
    Logging::LoggerPIMPL RenderWindow::logger{"input.txt"};
    bool RenderWindow::middleButtonDown = false;

    RenderWindow::RenderWindow(const char* windowTitle, int majorVersionNumber, int minorVersionNumber, int screenWidth, int screenHeight)
            :
                width{screenWidth},
                height{screenHeight},
                shouldClose{false}
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersionNumber);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersionNumber);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef DEBUG_MODE

            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        #endif

        window = glfwCreateWindow(screenWidth, screenHeight, windowTitle, nullptr, nullptr);

        if(window == nullptr)
        {
            glfwTerminate();

            throw std::runtime_error{"Unable to initialize GLFW in: " + std::string{__PRETTY_FUNCTION__}};
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, frameBufferSizeCalBack);
        glfwSetMouseButtonCallback(window, mouseButtonCallBack);
        glfwSetCursorPosCallback(window, mouseCallBack);
     //   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error{"Unable to initialize glad in: " + std::string{__PRETTY_FUNCTION__}};
        }

        #ifdef DEBUG_MODE

            GLint flags;

            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
            {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
                glDebugMessageCallback(MessageCallback, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }

        #endif

        camera.updateScreenSize(width, height);
    }

    const Camera::CameraObject &RenderWindow::getCamera() const
    {
        return camera;
    }

    void RenderWindow::pollEvents()
    {
        glfwPollEvents();
    }

    void RenderWindow::processInput()
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            shouldClose = true;

            logger.writeMessage("Pressed Escape");
        }

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.move(Camera::CameraMovement::Forwards);

            logger.writeMessage("Pressed W");
        }

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.move(Camera::CameraMovement::Left);

            logger.writeMessage("Pressed A");
        }

        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.move(Camera::CameraMovement::Backwards);

            logger.writeMessage("Pressed S");
        }

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.move(Camera::CameraMovement::Right);

            logger.writeMessage("Pressed D");
        }
    }

    void RenderWindow::swapBuffers()
    {
        glfwSwapBuffers(window);
    }

    bool RenderWindow::windowShouldClose()
    {
        return shouldClose;
    }

    void RenderWindow::terminate()
    {
        glfwTerminate();
    }

    // Beginning of private functions

    void RenderWindow::frameBufferSizeCalBack(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);

        camera.updateScreenSize(width, height);

        logger.writeMessage("Changed to size: " + std::to_string(width) + " , " + std::to_string(height));
    }

    void RenderWindow::mouseButtonCallBack(GLFWwindow *window, int button, int action, int mods)
    {
        if(button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            if(action == GLFW_PRESS)
            {
                middleButtonDown = true;
            }
            else
            {
                middleButtonDown = false;

                camera.resetFirstRotation();
            }
        }
    }

    void RenderWindow::mouseCallBack(GLFWwindow *window, double xPosition, double yPosition)
    {
        static unsigned int logCount = 0;

        logCount += 1;

        if(logCount == 10) // Prevent from too many printf being written to log.
        {
            logger.writeMessage("Cursor to: " + std::to_string(xPosition) + " , " + std::to_string(yPosition));

            logCount = 0;
        }

        if(middleButtonDown)
        {
            camera.rotate(xPosition, yPosition);
        }
    }

    // Debugging

    void GLAPIENTRY MessageCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar* message,
                                    const void* userParam)
    {

        fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                 ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                 type, severity, message );
    }
}