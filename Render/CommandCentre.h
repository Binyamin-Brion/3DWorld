//
// Created by binybrion on 6/8/20.
//

#ifndef GAMEPROTOTYPE_COMMANDCENTRE_H
#define GAMEPROTOTYPE_COMMANDCENTRE_H

#include "Shaders/InstanceShaderProgram.h"

namespace Render
{
    /**
     *  The single place where all of the required OpenGL functionality is centralized in. All OpenGL classes are located
     *  here and the order at which they execute their functionality is controlled by this class.
     */

    class CommandCentre
    {
        public:

            CommandCentre();

            /**
             *  Tells all required OpenGL classes to render their data. This function takes care of setting up OpenGL
             *  state before issuing the rendering calls.
             */
            void render();

        private:

            Shaders::InstanceShaderProgram instanceShaderProgram;

            unsigned int vao;
            unsigned int vbo;
    };
}

#endif //GAMEPROTOTYPE_COMMANDCENTRE_H
