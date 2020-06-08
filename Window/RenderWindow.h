//
// Created by binybrion on 6/8/20.
//

#ifndef GLFW_RENDERWINDOW_H
#define GLFW_RENDERWINDOW_H

#include <vector>
#include "../Logging/LoggerPIMPL.h"
#include <vec3.hpp>

class GLFWwindow;

namespace Window
{
    class RenderWindow
    {
        public:

            /**
             * Creates an OpenGL context and loads OpenGL functions.
             *
             * @param windowTitle name of the render window
             * @param majorVersionNumber major version number of OpenGL to use
             * @param minorVersionNumber minor version number of OpenGL to use
             * @param screenWidth width of the render window
             * @param screenHeight height of the render window
             */
            RenderWindow(const char* windowTitle, int majorVersionNumber, int minorVersionNumber, int screenWidth, int screenHeight);

            /**
             * Checks for any events that were given to the window.
             */
            void pollEvents();

            /**
             * Take appropriate action due to events given to the render window.
             */
            void processInput();


            /**
             * Swaps the buffers of the window.
             */
            void swapBuffers();

            /**
             * Mark the window as having to close.
             */
            void terminate();

            /**
             * Check if the window should be closed, ending the program.
             *
             * @return true if the window should be closed
             */
            bool windowShouldClose();

        private:

            /**
             * Callback for when the size of the window changes. The viewport is updated and the camera is updated to
             * represent the new size of the window.
             *
             * @param window pointer to the window that has changed size
             * @param width new width of the window
             * @param height new height of the window
             */
            static void frameBufferSizeCalBack(GLFWwindow *window, int width, int height);

            /**
             * Callback for cursor movement. The camera is rotated when this function is called assuming the game is an
             * appropriate state for such a change to the camera to occur.
             *
             * @param window pointer to the window that has changed size
             * @param xPosition x-position of the cursor
             * @param yPosition y-position of the cursor
             */
            static void mouseCallBack(GLFWwindow *window, double xPosition, double yPosition);

            GLFWwindow *window;
            bool shouldClose;
            int width;
            int height;

            static Logging::LoggerPIMPL logger;
    };
}

#endif //GLFW_RENDERWINDOW_H
