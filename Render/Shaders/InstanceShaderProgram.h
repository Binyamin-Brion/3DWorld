//
// Created by binybrion on 6/8/20.
//

#ifndef GAMEPROTOTYPE_INSTANCESHADERPROGRAM_H
#define GAMEPROTOTYPE_INSTANCESHADERPROGRAM_H

#include "ShaderProgramBase.h"

namespace Render::Shaders
{
    /**
     * Concrete instantiation of the ShaderProgramBase to create a shader program that deals with rendering instances of data.
     */

    class InstanceShaderProgram : public ShaderProgramBase
    {
        public:

            /**
             * Creates a shader program to render instances of data.
             */
            InstanceShaderProgram();
    };
}

#endif //GAMEPROTOTYPE_INSTANCESHADERPROGRAM_H
