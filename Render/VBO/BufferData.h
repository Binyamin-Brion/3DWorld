//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_BUFFERDATA_H
#define GAMEPROTOTYPE_BUFFERDATA_H

#include "glad.h"
#include <vector>

namespace Render::VBO
    {
        /**
         * A wrapper over a raw OpenGL VBO resource, providing an abstraction for creating and uploading data.
         *
         * Note: all of these functions must be called in the same thread as the OpenGL thread!
         *
         * @tparam T type of data the buffer will be storing
         * @tparam bufferTarget to be used for the buffer
         * @tparam bufferUsage to be used for the buffer
         */

        template<typename T, GLenum bufferTarget, GLenum bufferUsage>
        class BufferData
        {
            public:

                BufferData()
                {
                    glGenBuffers(1, &vbo);
                }

                /**
                 * Binds the raw OpenGL resource.
                 */
                void bind()
                {
                    glBindBuffer(bufferTarget, vbo);
                }

                /**
                 * Destroys the VBO resource. The VAO that references this VBO must be unbound first!
                 */
                void deleteVBO()
                {
                    glDeleteBuffers(1, &vbo);
                }

                /**
                 * Uploads the passed in data, overwriting any previous data.
                 *
                 * @param data to be uploaded into the VBO.
                 */
                void uploadData(const std::vector<T> &data)
                {
                    glBindBuffer(bufferTarget, vbo);
                    glBufferData(bufferTarget, data.size() * sizeof(T), data.data(), bufferUsage);
                }

            private:
                unsigned int vbo;
        };
    }

#endif //GAMEPROTOTYPE_BUFFERDATA_H
