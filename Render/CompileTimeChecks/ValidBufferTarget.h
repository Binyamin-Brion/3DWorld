//
// Created by binybrion on 3/20/20.
//

#ifndef GLFW_VALIDBUFFERTARGET_H
#define GLFW_VALIDBUFFERTARGET_H

#include "glad.h"

namespace Render::CompileTimeChecks
    {
        constexpr GLenum VALID_BUFFER_TARGETS_OPTIONS[] =
        {
                GL_ARRAY_BUFFER,
                GL_ATOMIC_COUNTER_BUFFER,
                GL_COPY_READ_BUFFER,
                GL_COPY_WRITE_BUFFER,
                GL_DISPATCH_INDIRECT_BUFFER,
                GL_DRAW_INDIRECT_BUFFER,
                GL_ELEMENT_ARRAY_BUFFER,
                GL_PIXEL_PACK_BUFFER,
                GL_PIXEL_UNPACK_BUFFER,
                GL_QUERY_BUFFER,
                GL_SHADER_STORAGE_BUFFER,
                GL_TEXTURE_BUFFER,
                GL_TRANSFORM_FEEDBACK_BUFFER,
                GL_UNIFORM_BUFFER
        };

        /**
         * This function returns, at compile time, whether the template parameter is one of the above flags.
         *
         * Note: This function does NOT detect invalid combinations of the above flag options. This makes the the function
         *       implementation harder, and the fact that guaranteeing such a possibility means carefully going over
         *       OpenGL documentation. Invalid options may be missed, and as such it is easier to just say this function
         *       does not offer such a promise.
         *
         * @tparam bufferFlags the flag to check if it is one of the above flags
         * @return true if the supplied template parameter is one of the above flgas
         */
        template<GLenum bufferTarget>
        constexpr bool VALID_BUFFER_TARGET()
        {
            for(const GLenum i : VALID_BUFFER_TARGETS_OPTIONS)
            {
                if(bufferTarget == i)
                {
                    return true;
                }
            }

            return false;
        }
    }

#endif //GLFW_VALIDBUFFERTARGET_H
