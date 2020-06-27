//
// Created by binybrion on 6/27/20.
//

#ifndef GAMEPROTOTYPE_SKYBOXSHADERPROGRAM_H
#define GAMEPROTOTYPE_SKYBOXSHADERPROGRAM_H

#include "ShaderProgramBase.h"

namespace Render::Shaders
{
    /**
     * Concrete instantiation of the SkyBoxShaderProgram to create a shader program that deals with rendering skyboxes.
     */

    class SkyBoxShaderProgram : public ShaderProgramBase
    {
        public:

            /**
             * Creates a shader program to render the skybox.
             */
            SkyBoxShaderProgram();
    };
}

#endif //GAMEPROTOTYPE_SKYBOXSHADERPROGRAM_H
