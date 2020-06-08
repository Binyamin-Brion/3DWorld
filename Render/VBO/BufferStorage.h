//
// Created by binybrion on 6/8/20.
//

#ifndef GAMEPROTOTYPE_BUFFERSTORAGE_H
#define GAMEPROTOTYPE_BUFFERSTORAGE_H


#include "glad.h"
#include "../CompileTimeChecks/ValidBufferTarget.h"
#include "../CompileTimeChecks/ValidBufferFlags.h"
#include "../CompileTimeChecks/ValidMappingFlags.h"

namespace Render::VBO
    {

        /**
         * Wraps an immutable buffer within a template class to provide compile-time checks on the configuration given to
         * the VBO. The only way to modify the VBO with data is to write through the mapped pointer; this allows changes
         * to be made in execution paths not in the OpenGL context, depending on how the VBO is configured.
         *
         * @tparam numPartitions number of partitions with partitionElementCount number of elements that the VBO will hold
         * @tparam bufferTarget the target to bind the buffer to
         * @tparam bufferUsage the usage bit pattern of the VBO
         */

        template<typename T, GLenum bufferTarget, GLbitfield bufferUsage, GLbitfield mapFlags>
        class BufferStorage
        {
            public:

                /**
                 * Initializes the buffer to hold the requested amount of data along with the configuration given.
                 *
                 * @param T the type of data the buffer will hold
                 * @param partitionElementCount the number of type T in each logical partition of the VBO
                 */
                explicit BufferStorage(unsigned int partitionElementCount)
                        :
                            maxRangeBytes{static_cast<unsigned int>(sizeof(T) * partitionElementCount)}
                {
                    static_assert(CompileTimeChecks::VALID_BUFFER_TARGET<bufferTarget>(),
                                  "An invalid buffer target was supplied to this buffer!");
                    static_assert(CompileTimeChecks::VALID_BUFFER_FLAGS<bufferUsage>(),
                                  "An invalid buffer usage flags was supplied to this buffer!");

                    glGenBuffers(1, &vbo);
                    glBindBuffer(bufferTarget, vbo);
                    glBufferStorage(bufferTarget, sizeof(T) * partitionElementCount, nullptr, bufferUsage);
                }

                /**
                 * Binds the buffer to the specified template bufferTarget parameter.
                 */
                void bind()
                {
                    glBindBuffer(bufferTarget, vbo);
                }

                /**
                 * Get a mapped pointer to the buffer. The entire VBO is mapped.
                 *
                 * Note: the flags specified for the mapping must be allowed with the VBO configuration given in the template class parameters.
                 *
                 * @tparam flags the bit pattern specifying how to map the buffer
                 * @return mapped pointer to the buffer
                 */
                void *getMappedBuffer()
                {
                    static_assert(CompileTimeChecks::VALID_MAPPING_FLAGS<mapFlags>(),
                                  "An invalid mapping option was specified!");

                    glBindBuffer(bufferTarget, vbo);

                    return glMapBufferRange(bufferTarget, 0, maxRangeBytes, mapFlags);
                }

            private:
                unsigned int vbo = 0;
                unsigned int maxRangeBytes;
        };
    }

#endif //GAMEPROTOTYPE_BUFFERSTORAGE_H
