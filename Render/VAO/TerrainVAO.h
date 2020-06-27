//
// Created by binybrion on 6/12/20.
//

#ifndef GAMEPROTOTYPE_TERRAINVAO_H
#define GAMEPROTOTYPE_TERRAINVAO_H

#include <vec3.hpp>
#include "../VBO/BufferData.h"
#include "VAOBase.h"

namespace Render::DataStructures
{
    class GridSectionInstanceRange;
}

namespace Render::Shaders
{
    class InstanceShaderProgram;
}

namespace Render::Textures
{
    class TextureManager;
}

namespace Render::VAO
{
    /**
     * Stores and render the terrain data.
     *
     * The terrain data is considered to be all of the surface cubes plus any gap filling cubes.
     */

    class TerrainVAO : public VAOBase
    {
        public:

            /**
             * Initializes the VAO by uploading the required model vertices needed to render the terrain.
             *
             * @param textureManager that holds all of the textures used in the program
             */
            explicit TerrainVAO(Textures::TextureManager &textureManager);

            /**
             * Renders the terrain that is within the viewing frustum.
             *
             * Note: This function will change uniforms in the passed in shader program!
             *
             * @param instanceShaderProgram used to render the program
             * @param visibleGridSections vector of visible grid section IDs
             */
            void render(Shaders::InstanceShaderProgram &instanceShaderProgram, const std::vector<unsigned int> &visibleGridSections);

            /**
             * Store the translations required for rendering in the buffer.
             *
             * @param translations of the cubes the terrain
             * @param gridSectionsInformation information about the number of model instances for each grid section
             */
            void uploadInstanceTranslations(const std::vector<glm::vec3> &translations, const std::vector<DataStructures::GridSectionInstanceRange> &gridSectionsInformation);

        private:

            VBO::BufferData<glm::vec3, GL_ARRAY_BUFFER, GL_STATIC_DRAW> cubeVertices;
            VBO::BufferData<unsigned int, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW> cubeIndices;
            VBO::BufferData<glm::vec3, GL_ARRAY_BUFFER, GL_STATIC_DRAW> instanceTranslations;
    };
}

#endif //GAMEPROTOTYPE_TERRAINVAO_H
