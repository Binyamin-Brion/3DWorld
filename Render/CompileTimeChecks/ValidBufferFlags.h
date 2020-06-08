//
// Created by binybrion on 3/20/20.
//

#ifndef GLFW_VALIDBUFFERFLAGS_H
#define GLFW_VALIDBUFFERFLAGS_H

#include "glad.h"

namespace Render::CompileTimeChecks
    {
        constexpr GLenum VALID_BUFFER_FLAGS_OPTIONS[] =
                {
                      GL_DYNAMIC_STORAGE_BIT,
                      GL_MAP_READ_BIT,
                      GL_MAP_WRITE_BIT,
                      GL_MAP_COHERENT_BIT,
                      GL_MAP_PERSISTENT_BIT,
                      GL_CLIENT_STORAGE_BIT
                };

        /**
         * This function returns, at compile time, whether the template parameter is a bit pattern of the above flag options.
         *
         * Note: This function does NOT detect invalid combinations of the above flag options. This makes the the function
         *       implementation harder, and the fact that guaranteeing such a possibility means carefully going over
         *       OpenGL documentation. Invalid options may be missed, and as such it is easier to just say this function
         *       does not offer such a promise.
         *
         * @tparam bufferFlags the bit pattern to check if it is an OR-ed combination of the above flags
         * @return true if the supplied template parameter is a valid bit pattern
         */

        template<GLbitfield bufferFlags>
        constexpr bool VALID_BUFFER_FLAGS()
        {
            // This function works using a powerset of the above flags. This allows to go over every possible combination
            // of OR-ing the above flags to see if the supplied template parameter matches at least one combination.
            // Note: since a combination is OR-ing, some sets within the powerset are redundant. These are still checked
            //       as it eases the implementation of this function.

            const unsigned int NUMBER_ELEMENTS = sizeof(VALID_BUFFER_FLAGS_OPTIONS) / sizeof(GLenum);

            // The number of possible sets in a powerset is 2^n.
            const unsigned int MAX_NUMBER_SETS = 1u << NUMBER_ELEMENTS;

            // The current set in the powerset. It goes up to 2^n.
            unsigned int powerSetNumber = 0;

            // Keep tracks of whether the current set in a powerset includes a certain flag.
            bool indexes[NUMBER_ELEMENTS] = {};

            while(powerSetNumber <= MAX_NUMBER_SETS)
            {
                // Bit pattern for this current set.
                GLenum currentBitPattern = 0;

                powerSetNumber += 1;

                for(bool &index : indexes)
                {
                    index = false;
                }

                // Any binary '1' in the powerset number is considered to include OR-ing a flag at the position of the binary '1'.
                for(unsigned int i = 0; i < NUMBER_ELEMENTS; ++i)
                {
                    if( (powerSetNumber >> i) & 1u)
                    {
                        indexes[i] = true;
                    }
                }

                // For any indexes value marked as true, then OR the associated flag
                for(int i = 0; i < NUMBER_ELEMENTS; ++i)
                {
                    if(indexes[i])
                    {
                        currentBitPattern |= VALID_BUFFER_FLAGS_OPTIONS[i];
                    }
                }

                if(currentBitPattern == bufferFlags)
                {
                    return true;
                }
            }

            return false;
        }
    }


#endif //GLFW_VALIDBUFFERFLAGS_H
