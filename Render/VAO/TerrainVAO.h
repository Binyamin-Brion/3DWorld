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
    class RenderInformation;
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
             * Store render data required to render static models in the world.
             *
             * Note: the actual translation data is only uploaded into vRAM after calling uploadInstanceTranslations().
             *
             * @param renderInformation the information to render instances of a single static model
             */
            void stageInstanceTranslations(const DataStructures::RenderInformation &renderInformation);

            /**
             * All instance data that was staged is uploaded into vRam.
             */
            void uploadInstanceTranslations();

        private:

            VBO::BufferData<glm::vec3, GL_ARRAY_BUFFER, GL_STATIC_DRAW> cubeVertices;
            VBO::BufferData<unsigned int, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW> cubeIndices;
            VBO::BufferData<glm::vec3, GL_ARRAY_BUFFER, GL_STATIC_DRAW> instanceTranslations;

            std::vector<glm::vec3> instanceTranslationsData;
    };
}

#endif //GAMEPROTOTYPE_TERRAINVAO_H
