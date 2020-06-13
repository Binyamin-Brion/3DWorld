//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_VAOBASE_H
#define GAMEPROTOTYPE_VAOBASE_H

#include <experimental/filesystem>

namespace Render::VAO
{
    /**
     * Base class from which all VAOs inherit from. Provides the most basic functionality required for a VAO as well
     * as any helper functions common to all VAOs.
     */

    class VAOBase
    {
        public:

            /**
             * Binds the VAO.
             */
            void bind();

        protected:

            /**
             * Creates the VAO with the required OpenGL call. This constructor is protected as no BaseVAO should be created;
             * only VAOs that inherit from this class.
             */
            VAOBase();

            /**
             * Get the folder holding the model files used for rendering.
             *
             * @return asset model folder location
             */
            std::experimental::filesystem::path getModelAssetFolder();

            unsigned vao;
    };
}

#endif //GAMEPROTOTYPE_VAOBASE_H
