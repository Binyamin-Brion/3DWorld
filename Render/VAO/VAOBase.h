//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_VAOBASE_H
#define GAMEPROTOTYPE_VAOBASE_H

#include <experimental/filesystem>
#include "../DataStructures/ModelRenderInformation.h"
#include "../VBO/BufferData.h"
#include "../DataStructures/RenderStructure.h"

#include <vec3.hpp>
#include <vec2.hpp>

namespace Render::Textures
{
    class TextureManager;
}

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

            /**
             * Loads the model at the given location and stores the model information into the appropriate buffers within
             * this class.
             *
             * @param textureManager that holds all of the textures used in the program
             * @param modelLocation vector of locations on the file disk of the model file
             */
            void loadModel(Textures::TextureManager &textureManager, const std::vector<std::string> &modelLocations);

            // All of the rendering data all of the models loaded.
            std::vector<DataStructures::ModelRenderingInformation> modelRenderingInformation;

            DataStructures::RenderStructure renderStructure;

        private:

            // TODO: Depending on future VAOs created, these may have to be protected, not private.

            unsigned vao;
            VBO::BufferData<glm::vec3, GL_ARRAY_BUFFER, GL_STATIC_DRAW> vertices;
            VBO::BufferData<unsigned int, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW> indices;
            VBO::BufferData<glm::vec2, GL_ARRAY_BUFFER, GL_STATIC_DRAW> textureCoordinates;

            unsigned int verticesLoaded = 0;
            unsigned int indicesLoaded = 0;
    };
}

#endif //GAMEPROTOTYPE_VAOBASE_H
